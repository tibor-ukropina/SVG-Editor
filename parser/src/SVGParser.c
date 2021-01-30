/* 
Tibor Ukropina
Student ID: 0961384

Citations:
Starter Code (Provided by prof)
http://knol2share.blogspot.com/2009/05/validate-xml-against-xsd-in-c.html
http://www.xmlsoft.org/examples/tree2.c
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "SVGParser.h"
#include "LinkedListAPI.h"

int addTwo(int n){
    return n+2;
}

// makes sure all lists in struct are initialized
bool validStruct(SVGimage *img){

    ListIterator iter;
    ListIterator aiter;
    void *elem;
    void *aelem;


    if( img->otherAttributes == NULL || img->rectangles == NULL || img->circles == NULL ||
        img->paths == NULL || img->groups == NULL || img->title == NULL || img->description == NULL
    ){
        return false;
    }

    if(getLength(img->rectangles) != 0){
        Rectangle *rectangle;
        iter = createIterator(img->rectangles);

        while((elem = nextElement(&iter)) != NULL){

            rectangle = (Rectangle*)elem;
            if(rectangle->otherAttributes == NULL){
                return false;
            }
            if(rectangle->units == NULL){
                return false;
            }
            Attribute *atr;
            aiter = createIterator(rectangle->otherAttributes);

            while((aelem = nextElement(&aiter)) != NULL){
                
                atr = (Attribute*)aelem;
                
                if(atr->name == NULL){
                    return false;
                }
                if(atr->value == NULL ){
                    return false;
                }
            }
        }
    }
    if(getLength(img->circles) != 0){
        Circle *circle;
        iter = createIterator(img->circles);

        while((elem = nextElement(&iter)) != NULL){

            circle = (Circle*)elem;
            if(circle->otherAttributes == NULL){
                return false;
            }
            if(circle->units == NULL){
                return false;
            }
            Attribute *atr;
            aiter = createIterator(circle->otherAttributes);

            while((aelem = nextElement(&aiter)) != NULL){
                
                atr = (Attribute*)aelem;
                
                if(atr->name == NULL){
                    return false;
                }
                if(atr->value == NULL ){
                    return false;
                }
            }
        }
    }
    if(getLength(img->paths) != 0){
        Path *path;
        iter = createIterator(img->paths);

        while((elem = nextElement(&iter)) != NULL){

            path = (Path*)elem;
            if(path->otherAttributes == NULL){
                return false;
            }
            Attribute *atr;
            aiter = createIterator(path->otherAttributes);

            while((aelem = nextElement(&aiter)) != NULL){
                
                atr = (Attribute*)aelem;
                
                if(atr->name == NULL){
                    return false;
                }
                if(atr->value == NULL ){
                    return false;
                }
            }
        }
    }
    if(getLength(img->groups) != 0){
        Group *group;
        ListIterator iter = createIterator(img->groups);

        while((elem = nextElement(&iter)) != NULL){

            group = (Group*)elem;
            if(group->otherAttributes == NULL){
                return false;
            }
            Attribute *atr;
            aiter = createIterator(group->otherAttributes);

            while((aelem = nextElement(&aiter)) != NULL){
                
                atr = (Attribute*)aelem;
                
                if(atr->name == NULL){
                    return false;
                }
                if(atr->value == NULL ){
                    return false;
                }
            }
        }
    }
    Attribute *atr;
    aiter = createIterator(img->otherAttributes);

    while((aelem = nextElement(&aiter)) != NULL){
        
        atr = (Attribute*)aelem;
        
        if(atr->name == NULL){
            return false;
        }
        if(atr->value == NULL ){
            return false;
        }
    }
    return true;
}

//delete function to avoid double free
void dummyDelete(void* data){}

//populate circle struct
Circle *createCircle(xmlNode *cur_node){

    int flag = 0;
    char *punits;
    Circle *newCircle = malloc(sizeof(Circle));
    newCircle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(newCircle->units, "");
    newCircle->cx = 0;
    newCircle->cy = 0;
    newCircle->r = 0;

    // Iterate through every attribute of the current node
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if(strcmp(attrName, "cx") == 0){
            
            newCircle->cx = strtof(cont, &punits);
            if(punits[0] != '\0' && flag == 0){
                strcpy(newCircle->units, punits);
                flag++;
            }
        }
        else if(strcmp(attrName, "cy") == 0){
            
            newCircle->cy = strtof(cont, &punits);
            if(punits[0] != '\0' && flag == 0){
                strcpy(newCircle->units, punits);
                flag++;
            }
        }
        else if(strcmp(attrName, "r") == 0){

            /* check for negative values */
            if(cont[0] == '-'){
                deleteCircle(newCircle);
                return NULL;
            }

            newCircle->r = strtof(cont, &punits);
            
            if(punits[0] != '\0' && flag == 0){
                strcpy(newCircle->units, punits);
                flag++;
            }
        }
        else{

            Attribute *atr = malloc(sizeof(Attribute));
            atr->name = malloc(1000);

            strcpy(atr->name, attrName);
            atr->value = malloc(1000);
            
            strcpy(atr->value, cont);
            insertBack(newCircle->otherAttributes, atr);
        }
    }

    return newCircle;

}

// populate rectangle struct 
Rectangle *createRect(xmlNode *cur_node){

    int flag = 0;
    char* units;
    Rectangle *newRect = malloc(sizeof(Rectangle));
    newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    newRect->x = 0;
    newRect->y = 0;
    newRect->width = 0;
    newRect->height = 0;
    strcpy(newRect->units, "");

    // Iterate through every attribute of the current node
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if(strcmp(attrName, "x") == 0){
            
            newRect->x = strtof(cont, &units);
            if(units[0] != '\0' && flag == 0){
                strcpy(newRect->units, units);
                flag++;
            }
        }
        else if(strcmp(attrName, "y") == 0){
            
            newRect->y = strtof(cont, &units);
            if(units[0] != '\0' && flag == 0){
                strcpy(newRect->units, units);
                flag++;
            }
        }
        else if(strcmp(attrName, "width") == 0){
            
            if(cont[0] == '-'){
                deleteRectangle(newRect);
                return NULL;
            }
            newRect->width = strtof(cont, &units);
            if(units[0] != '\0' && flag == 0){
                strcpy(newRect->units, units);
                flag++;
            }
        }
        else if(strcmp(attrName, "height") == 0){
            
            if(cont[0] == '-'){
                deleteRectangle(newRect);
                return NULL;
            }
            newRect->height = strtof(cont, &units);
            if(units[0] != '\0' && flag == 0){
                strcpy(newRect->units, units);
                flag++;
            }
        }
        else{

            Attribute *atr = malloc(sizeof(Attribute));

            atr->name = malloc(10000);
            strcpy(atr->name, attrName);

            atr->value = malloc(10000);
            strcpy( atr->value, cont);

            insertBack(newRect->otherAttributes, atr);
        }
    }

    return newRect;

}


// populate path struct
Path *createPath(xmlNode *cur_node){

    Path *newPath = malloc(sizeof(Path));
    newPath->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);
    // strcpy(newPath->data, "");

    // Iterate through every attribute of the current node
    xmlAttr *attr;
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        if(strcmp(attrName, "d") == 0){
            
            newPath->data = malloc(strlen(cont) + 1000);
            strcpy(newPath->data, cont);
            
        }
        else{
            Attribute *atr = malloc(sizeof(Attribute));

            atr->name = malloc(strlen(attrName) + 1000);
            strcpy( atr->name, attrName);

            atr->value = malloc(strlen(cont) + 1000);
            strcpy(atr->value, cont);

            insertBack(newPath->otherAttributes, atr);
        }
    }

    return newPath;

}

//populate group struct
Group *createGroup(xmlNode **a_node, xmlNode *b_node){

    Group *newGroup = malloc(sizeof(Group));
    xmlNode *cur_node = NULL;
    char* buffer;
    xmlAttr *attr;

    //initialize lists for group
    newGroup->circles = initializeList(circleToString, deleteCircle, compareCircles);
    newGroup->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    newGroup->paths = initializeList(pathToString, deletePath, comparePaths);
    newGroup->groups = initializeList(groupToString, deleteGroup, compareGroups);
    newGroup->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    // go through current node to find other attributes
    for (attr = b_node->properties; attr != NULL; attr = attr->next)
    {
        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        Attribute *atr = malloc(sizeof(Attribute));
        atr->name = malloc(1000);
        
        strcpy( atr->name, attrName);
        atr->value = malloc(1000);
        
        strcpy(atr->value, cont);
        insertBack(newGroup->otherAttributes, atr);
    }

    for (cur_node = *a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            
            buffer = (char *)cur_node->name;
            
            if(strcmp(buffer, "circle")==0){
                insertBack(newGroup->circles, createCircle(cur_node));
            }
            else if(strcmp(buffer, "rect")==0){
                insertBack(newGroup->rectangles, createRect(cur_node));
            }
            else if(strcmp(buffer, "path")==0){
                //printf("\t");
                insertBack(newGroup->paths, createPath(cur_node));
            }
            else if(strcmp(buffer, "g")==0){
                insertBack(newGroup->groups, createGroup(&cur_node->children, cur_node));
                //continue;
            }
        }
    }
    
    // printf("%s\n", groupToString(newGroup));
    // printf("%s\n", toString(newGroup->rectangles));
    return newGroup;
}

// seperate function for traversing svg file
SVGimage* traversal(xmlNode * a_node, SVGimage* img){

    char* buffer;
    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {

        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            buffer = (char *)cur_node->name;

            if(!cur_node->ns){
                deleteSVGimage(img);
                return NULL;
            }
            if(cur_node->ns){
                strncpy(img->namespace, (char*)cur_node->ns->href, 255);
            }

            if(strcmp(buffer, "title")==0){
                if(cur_node->children != NULL){
                    strncpy(img->title, (char*)cur_node->children->content, 255);
                }
            }
            if(strcmp(buffer, "desc")==0){
                if(cur_node->children != NULL){
                    strncpy(img->description, (char*)cur_node->children->content, 255);
                }
            }
            if(strcmp(buffer, "circle")==0){
                Circle *circle;
                circle = createCircle(cur_node);

                if(circle != NULL){
                    insertBack(img->circles, circle);
                }
            }
            else if(strcmp(buffer, "rect")==0){
                insertBack(img->rectangles, createRect(cur_node));
            }
            else if(strcmp(buffer, "path")==0){
                insertBack(img->paths, createPath(cur_node));
            }
            else if(strcmp(buffer, "g")==0){
                insertBack(img->groups, createGroup(&cur_node->children, cur_node));  
                continue;  
            }
        }
        
        traversal(cur_node->children, img);
    }
    return img;
}

SVGimage* createSVGimage(char* fileName){


    xmlDoc *doc = xmlReadFile(fileName, NULL, 0);
    xmlNode *root_element = NULL;
    
    /* check for empty document */
    if(doc == NULL){
        xmlFreeDoc(doc);
        return NULL;
    }
    if(fileName == NULL){
        xmlFreeDoc(doc);
        return NULL;
    }
    SVGimage* img = malloc(sizeof(SVGimage));

    // /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    /* initialize lists for the image */
    img->circles = initializeList(circleToString, deleteCircle, compareCircles);
    img->paths = initializeList(pathToString, deletePath, comparePaths);
    img->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    img->groups = initializeList(groupToString, deleteGroup, compareGroups);
    img->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    xmlAttr *attr;

    for (attr = root_element->properties; attr != NULL; attr = attr->next){

        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        Attribute *atr = malloc(sizeof(Attribute));

        atr->name = malloc(1000);
        strcpy( atr->name, attrName);

        atr->value = malloc(1000);
        strcpy(atr->value, cont);

        insertBack(img->otherAttributes, atr);
    }

    strcpy(img->title, "");
    strcpy(img->description, "");

    /* traverse the document*/
    img = traversal(root_element, img);

    //free libxml2 variables
    xmlFreeDoc(doc);
    xmlCleanupParser();

    return img;

}

void deleteSVGimage(SVGimage* img){

    if(img){
        if(img->circles){
            freeList(img->circles);
        }
        if(img->rectangles){
            freeList(img->rectangles);
        }
        if(img->paths){
            freeList(img->paths);
        }
        if(img->groups){
            freeList(img->groups);
        }
        if(img->otherAttributes){
            freeList(img->otherAttributes);
        }
        if(img){
            free(img);
        }
    }
}


/* toString Functions*/
char* SVGimageToString(SVGimage* img){


    char* toString = malloc(1000);

    if(img){
        char circles[100];
        char rectangles[100];
        char paths[100];
        char groups[100];

        sprintf(toString, "Image: \n");
        strcat(toString, "Title: ");
        strcat(toString, img->title);
        strcat(toString, "\n");
        strcat(toString, "Namespace:");
        strcat(toString, img->namespace);
        strcat(toString, "\n");

        
        strcat(toString, "Description:");
        strcat(toString, img->description);
        strcat(toString, "\n");


        sprintf(circles, "Circles: %d\n", getLength(img->circles));
        strcat(toString, circles);

        sprintf(rectangles, "Rectangles: %d\n", getLength(img->rectangles));
        strcat(toString, rectangles);

        sprintf(paths, "Paths: %d\n", getLength(img->paths));
        strcat(toString, paths);

        sprintf(groups, "Groups: %d\n", getLength(img->groups));
        strcat(toString, groups);

        return toString;
    }

    sprintf(toString, "NULL");
    return toString;
    
}


char* groupToString( void* data){

    Group* new;
    new = (Group*)data;
    
    char* toString = malloc(1000);
    char circles[100];
    char rectangles[100];
    char paths[100];
    char groups[100];

    if(data){

        sprintf(toString, "Group:\n");

        if(new->circles){
            sprintf(circles, "Circles: %d\n", getLength(new->circles));
            strcat(toString, circles);
        }
        if(new->rectangles){
            sprintf(rectangles, "Rectangles: %d\n", getLength(new->rectangles));
            strcat(toString, rectangles);
        }
        if(new->paths){
            sprintf(paths, "Paths: %d\n", getLength(new->paths));
            strcat(toString, paths);
        }
        if(new->groups){
            sprintf(groups, "Groups: %d\n", getLength(new->groups));
            strcat(toString, groups);
        }
    }

    sprintf(toString, "NULL");
    return toString;
}

char* circleToString(void* data){
    
    Circle* new;
    new = (Circle*)data;
    char* toString = malloc(1000);
    char radius[100];
    char x[100];
    char y[100];
    char units[100];

    if(data){

        sprintf(toString, "Circle: \n");

        if(new){

            sprintf(radius, "Radius = %f\n", new->r);
            strcat(toString, radius);

            sprintf(x, "x = %f\n", new->cx);
            strcat(toString, x);

            sprintf(y, "y = %f\n", new->cy);
            strcat(toString, y);

            sprintf(units, "units = %s\n", new->units);
            strcat(toString, units);

            return toString;
        }
    }

    sprintf(toString, "NULL");
    return toString;
}

char* rectangleToString(void* data){

    Rectangle* new;
    new = (Rectangle*)data;
    char* toString = malloc(1000);
    char x[100];
    char y[100];
    char height[100];
    char width[100];
    char units[100];

    

    if(data){

        sprintf(toString, "Rectangle: \n");

        sprintf(x, "x = %f\n", new->x);
        strcat(toString, x);

        sprintf(y, "y = %f\n", new->y);
        strcat(toString, y);
        
        sprintf(height, "x = %f\n", new->height);
        strcat(toString, height);

        sprintf(width, "y = %f\n", new->width);
        strcat(toString, width);

        sprintf(units, "units = %s\n", new->units);
        strcat(toString, units);

        return toString;
    }

    sprintf(toString, "NULL");
    return toString;
}

char* pathToString(void* data){

    Path* new;
    new = (Path*)data;
    char* toString = malloc(1000);
    char pdata[500];

    if(data){

        sprintf(toString, "Path: \n");

        if(new){
            sprintf(pdata, "Data = %s\n", new->data);
            strcat(toString, pdata);
        }
    }

    sprintf(toString, "NULL");
    return toString;
}

char* attributeToString( void* data){

    
    
    Attribute* new;
    new = (Attribute*)data;
    char* toString = malloc(1000);
    char name[500];
    char value[500];

    if(data){

        sprintf(toString, "Attribute: \n");

        if(new){
            sprintf(name, "Name = %s\n", new->name);
            strcat(toString, name);

            sprintf(value, "Value = %s\n", new->value);
            strcat(toString, value);
        }
    }

    sprintf(toString, "NULL");
    return toString;
}

/* delete functions */

void deleteGroup(void* data){

    Group* new;

    new = (Group*)data;


    if(new){
        if(new->circles){
            freeList(new->circles);
        }
        if(new->rectangles){
            freeList(new->rectangles);
        }
        if(new->paths){
            freeList(new->paths);
        }
        if(new->otherAttributes){
            freeList(new->otherAttributes);
        }
        if(new->groups){
            freeList(new->groups);
        }
        free(new);
    }
    
}


void deleteCircle(void* data){

    Circle* new;

    new = (Circle*)data;

    if(new){
        if(new->otherAttributes){
            freeList(new->otherAttributes);
        }
        free(new);
    }
}

void deleteRectangle(void* data){
    
    Rectangle* new;

    new = (Rectangle*)data;
    
    if(new){
        if(new->otherAttributes){
            freeList(new->otherAttributes);
        }
        free(new);
    }    
}


void deletePath(void* data){

    Path* new;

    new = (Path*)data;

    if(new){
        if(new->data){
            free(new->data);
        }
        if(new->otherAttributes){
            freeList(new->otherAttributes);
        }
        free(new);
    }
}

void deleteAttribute( void* data){

    Attribute* new;
    new = (Attribute*)data;
    
    if(new){
        if(new->name){
            free(new->name);
        }
        if(new->value){
            free(new->value);
        }
        free(new);
    }
}


/* compare functions */
int compareGroups(const void *first, const void *second){
    return 0;
}

int compareCircles(const void *first, const void *second){
    return 0;
}

int compareRectangles(const void *first, const void *second){
    return 0;
}

int comparePaths(const void *first, const void *second){
    return 0;
}

int compareAttributes(const void *first, const void *second){
    return 0;
}

List* getRectsInGroup(List* rects, Group *group){

    ListIterator iter;
    Group *agroup;
    Rectangle *rectangle;
    void* elem;
    
    if(group->rectangles){
        iter = createIterator(group->rectangles);

	    while((elem = nextElement(&iter)) != NULL){
            rectangle = (Rectangle*)elem;
            insertBack(rects, rectangle);
        }
    }
    if(group->groups){
        iter = createIterator(group->groups);

	    while((elem = nextElement(&iter)) != NULL){
            agroup = (Group*)elem;
            //printf("%s\n", groupToString(agroup));
            if( getLength(agroup->rectangles) != 0 || getLength(agroup->groups) != 0){
                //printf("%s\n", toString(agroup->rectangles));
                rects = getRectsInGroup(rects, agroup);
            }
        }
    }

    return rects;
}

List* getRects(SVGimage* img){

    if(img == NULL){
        return NULL;
    }

    ListIterator iter;
    void *elem;
    Group *group;
    Rectangle *rectangle;
    List* rects = initializeList(rectangleToString, dummyDelete, compareRectangles);

    //add rectangles list from the image into the new list
    if(img){

        if(getLength(img->rectangles) != 0){
            ListIterator iter = createIterator(img->rectangles);

            while((elem = nextElement(&iter)) != NULL){

                rectangle = (Rectangle*)elem;
                insertBack(rects, rectangle);
            }
        }

        if(getLength(img->groups) != 0 ){
            iter = createIterator(img->groups);

            while((elem = nextElement(&iter)) != NULL){
                group = (Group*)elem;
                if(group->groups || group->rectangles){
                    rects = getRectsInGroup(rects, group);
                }
            }
        }
    }

    return rects;
}

List* getCirclesInGroup(List* circs, Group *group){

    Group *agroup;
    Circle *circles;
    
    if(getLength(group->circles) != 0){
        ListIterator iter = createIterator(group->circles);
        void* elem;

	    while((elem = nextElement(&iter)) != NULL){
            circles = (Circle*)elem;
            insertBack(circs, circles);
        }
    }
    if(getLength(group->groups) != 0){
        ListIterator giter = createIterator(group->groups);
        void* gelem;

	    while((gelem = nextElement(&giter)) != NULL){
            agroup = (Group*)gelem;
            //printf("%s\n", groupToString(agroup));
            if(agroup->circles || agroup->groups){
                //printf("%s\n", toString(agroup->circles));
                circs = getCirclesInGroup(circs, agroup);
            }
        }
    }

    return circs;
}

List* getCircles(SVGimage* img){

    if(img == NULL){
        return NULL;
    }

    Group *group;
    Circle *circles;
    List* circs = initializeList(circleToString, dummyDelete, compareCircles);

    //add circles list from the image into the new list
    if(img){

        if(getLength(img->circles) != 0){
            ListIterator riter = createIterator(img->circles);
            void* relem; 

            while((relem = nextElement(&riter)) != NULL){

                circles = (Circle*)relem;
                insertBack(circs, circles);
            }
        }

        if(getLength(img->groups) != 0){
            ListIterator iter = createIterator(img->groups);
            void* elem; 

            while((elem = nextElement(&iter)) != NULL){
                group = (Group*)elem;
                circs = getCirclesInGroup(circs, group);
            }
        }
    }

    return circs;
}

List* getGroupsInGroup(List* g, Group *group){

    Group *agroup;
    
    if(group->groups){
        ListIterator giter = createIterator(group->groups);
        void* gelem;

	    while((gelem = nextElement(&giter)) != NULL){
            agroup = (Group*)gelem;
            //printf("%s\n", groupToString(agroup));
            if(agroup->groups){
                //printf("%s\n", toString(agroup->rectangles));
                insertBack(g, agroup);
                g = getGroupsInGroup(g, agroup);
            }
        }
    }
    return g;
}

List* getGroups(SVGimage* img){

    if(img == NULL){
        return NULL;
    }
    
    Group *group;
    List* g = initializeList(groupToString, dummyDelete, compareGroups);

    //add rectangles list from the image into the new list
    if(img){

        if(img->groups){
            ListIterator iter = createIterator(img->groups);
            void* elem; 

            while((elem = nextElement(&iter)) != NULL){
                group = (Group*)elem;

                insertBack(g, group);
                g = getGroupsInGroup(g, group);
            }
        }
    }

    return g;
}


List* getPathsInGroup(List* p, Group *group){

    Group *agroup;
    Path *path;
    
    if(group->paths){
        ListIterator iter = createIterator(group->paths);
        void* elem;

	    while((elem = nextElement(&iter)) != NULL){
            path = (Path*)elem;
            insertBack(p, path);
        }
    }
    if(group->groups){
        ListIterator giter = createIterator(group->groups);
        void* gelem;

	    while((gelem = nextElement(&giter)) != NULL){
            agroup = (Group*)gelem;
            //printf("%s\n", groupToString(agroup));
            if(agroup->paths || agroup->groups){
                //printf("%s\n", toString(agroup->paths));
                p = getPathsInGroup(p, agroup);
            }
        }
    }
    return p;
}

List* getPaths(SVGimage* img){

    if(img == NULL){
        return NULL;
    }
   
    Group *group;
    Path *path;
    List* p = initializeList(pathToString, dummyDelete, comparePaths);

    //add paths list from the image into the new list
    if(img){
        if(getLength(img->paths) != 0){
            ListIterator riter = createIterator(img->paths);
            void* relem; 

            while((relem = nextElement(&riter)) != NULL){

                path = (Path*)relem;
                insertBack(p, path);
            }
        }

        if(getLength(img->groups)!= 0){
            ListIterator iter = createIterator(img->groups);
            void* elem; 

            while((elem = nextElement(&iter)) != NULL){
                group = (Group*)elem;
                p = getPathsInGroup(p, group);
            }
        }
    }

    return p;
}

int numRectsWithArea(SVGimage* img, float area){

    Rectangle *rectangle;
    int ptr, temp, h, w, count;
    List *list = getRects(img);

    if(img == NULL){
        return 0;
    }

    ptr = ceil(area);
    count = 0;

    ListIterator iter = createIterator(list);
    void* elem; 

    while((elem = nextElement(&iter)) != NULL){
        rectangle = (Rectangle*)elem;
        
        h = ceil(rectangle->height);
        w = ceil(rectangle->width);
        temp = h * w;

        if(ptr == temp){
            ++count;
        }
    }

    freeList(list);
    return count;
}

int numCirclesWithArea(SVGimage* img, float area){

    float pi = 3.14159265359;
    Circle *circle;
    int count;
    float temp;
    List *list = getCircles(img);

    if(img == NULL){
        return 0;
    }

    area = ceil(area);
    count = 0;

    ListIterator iter = createIterator(list);
    void* elem; 

    while((elem = nextElement(&iter)) != NULL){
        circle = (Circle*)elem;
        
        temp = pi * circle->r * circle->r;
        temp = ceil(temp);

        if(area == temp){
            ++count;
        }
    }

    freeList(list);
    return count;
}

int numPathsWithdata(SVGimage* img, char* data){

    Path *path;
    int count;
    List *list = getPaths(img);

    if(img == NULL){
        return 0;
    }

    count = 0;

    ListIterator iter = createIterator(list);
    void* elem; 

    while((elem = nextElement(&iter)) != NULL){
        path = (Path*)elem;
        
        if(path->data){
            if( strcmp(data, path->data) == 0 ){
            ++count;
            }
        }
    }
    freeList(list);
    return count;
}

int numGroupsWithLen(SVGimage* img, int len){

    if(img == NULL){
        return 0;
    }
    
    List *groups = getGroups(img);
    int count, length;
    Group *group;
    ListIterator iter = createIterator(groups);
    void* elem; 
    
    count = 0;

    if(img){
        while((elem = nextElement(&iter)) != NULL){

            length = 0;
            group = (Group*)elem;
            
            length += getLength(group->paths);
            length += getLength(group->circles);
            length += getLength(group->rectangles);
            length += getLength(group->groups);

            if(length == len){
                count++;
            }
        }
    }

    freeList(groups);
    return count;
}

int numAttr(SVGimage* img){


    int atrCount = 0;

    Group *group;
    Rectangle *rectangle;
    Path *path;
    Circle *circle;

    if(img == NULL){
        return 0;
    }

    List *groups = getGroups(img);
    List *rectangles = getRects(img);
    List *paths = getPaths(img);
    List *circles = getCircles(img);

    ListIterator giter = createIterator(groups);
    ListIterator riter = createIterator(rectangles);
    ListIterator piter = createIterator(paths);
    ListIterator citer = createIterator(circles);
    
    void* gelem;
    while((gelem = nextElement(&giter)) != NULL){

        group = (Group*)gelem;
        if(group->otherAttributes){
            atrCount += getLength(group->otherAttributes);  
        } 
    }

    void* relem;
    while((relem = nextElement(&riter)) != NULL){

        rectangle = (Rectangle*)relem;
        if(rectangle->otherAttributes){
            atrCount += getLength(rectangle->otherAttributes);
        }
    }

    void* pelem;
    while((pelem = nextElement(&piter)) != NULL){

        path = (Path*)pelem;
        if(path->otherAttributes){
            atrCount += getLength(path->otherAttributes);
        }
    }

    void* celem;
    while((celem = nextElement(&citer)) != NULL){

        circle = (Circle*)celem;
        if(circle->otherAttributes){
            atrCount += getLength(circle->otherAttributes);
        }
    }

    if(img){
        atrCount += getLength(img->otherAttributes);
    }

    freeList(groups);
    freeList(rectangles);
    freeList(paths);
    freeList(circles);

    return atrCount;
}

/* --------------- Assignment 2 Functions ----------------- */


void makeGroup(Group *group, xmlNodePtr xnode){

    xmlNodePtr node;

    if(getLength(group->rectangles) != 0){

        Rectangle *rectangle;
        ListIterator riter = createIterator(group->rectangles);
        void* relem; 

        while((relem = nextElement(&riter)) != NULL){

            char x[100];
            char y[100];
            char width[100];
            char height[100];
            char units[100];

            rectangle = (Rectangle*)relem;
            node = xmlNewChild(xnode, NULL, BAD_CAST "rect", NULL);

            sprintf(units, "%s", rectangle->units);
            sprintf(x, "%0.2f", rectangle->x);
            strcat(x, units);
            sprintf(y, "%0.2f", rectangle->y);
            strcat(y, units);
            sprintf(width, "%0.2f", rectangle->width);
            strcat(width, units);
            sprintf(height, "%0.2f", rectangle->height);
            strcat(height, units);

            xmlNewProp(node, BAD_CAST "x", BAD_CAST x);
            xmlNewProp(node, BAD_CAST "y", BAD_CAST y);
            xmlNewProp(node, BAD_CAST "width", BAD_CAST width);
            xmlNewProp(node, BAD_CAST "height", BAD_CAST height);

        }
    }
    if(getLength(group->circles) != 0 ){

        Circle *circle;
        ListIterator citer = createIterator(group->circles);
        void* celem; 

        while((celem = nextElement(&citer)) != NULL){
            
            char cx[100];
            char cy[100];
            char r[100];
            char units[100];

            circle = (Circle*)celem;
            node = xmlNewChild(xnode, NULL, BAD_CAST "circle", NULL);

            sprintf(units, "%s", circle->units);
            sprintf(cx, "%0.2f", circle->cx);
            strcat(cx, units);
            sprintf(cy, "%0.2f", circle->cy);
            strcat(cy, units);
            sprintf(r, "%0.2f", circle->r);
            strcat(r, units);

            xmlNewProp(node, BAD_CAST "cx", BAD_CAST cx);
            xmlNewProp(node, BAD_CAST "cy", BAD_CAST cy);
            xmlNewProp(node, BAD_CAST "r", BAD_CAST r);

            if(circle->otherAttributes){
                ListIterator citer2 = createIterator(circle->otherAttributes);
                void* celem2; 

                while((celem2 = nextElement(&citer2)) != NULL){

                    char name[1000];
                    char value[1000];

                    Attribute* atr = (Attribute*)celem2;

                    sprintf(name, "%s", atr->name);
                    sprintf(value, "%s", atr->value);

                    xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                }
            }
        }
    }
    if(getLength(group->paths) != 0){

        Path *path;
        ListIterator piter = createIterator(group->paths);
        void* pelem; 

        while((pelem = nextElement(&piter)) != NULL){

            path = (Path*)pelem;
            node = xmlNewChild(xnode, NULL, BAD_CAST "path", NULL);
            
            
            if(getLength(path->otherAttributes)!= 0){
                ListIterator piter2 = createIterator(path->otherAttributes);
                void* pelem2; 

                while((pelem2 = nextElement(&piter2)) != NULL){

                    char name[1000];
                    char value[1000];

                    Attribute* atr = (Attribute*)pelem2;

                    sprintf(name, "%s", atr->name);
                    sprintf(value, "%s", atr->value);

                    xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                }
            }
            if(path->data){
                xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);
            }
        }
    }
    if(getLength(group->groups) != 0){
        
        xmlNodePtr node;
        Group *newGroup;
        ListIterator giter = createIterator(group->groups);
        void* gelem; 

        while((gelem = nextElement(&giter)) != NULL){

            newGroup = (Group*)gelem;
            node = xmlNewChild(xnode, NULL, BAD_CAST "g", NULL);

            if(newGroup->otherAttributes){
                ListIterator giter2 = createIterator(newGroup->otherAttributes);
                void* gelem2; 

                while((gelem2 = nextElement(&giter2)) != NULL){

                    char name[1000];
                    char value[1000];

                    Attribute* atr = (Attribute*)gelem2;

                    sprintf(name, "%s", atr->name);
                    sprintf(value, "%s", atr->value);

                    xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                }
            }

            makeGroup(newGroup, node);
        }
    }
}

xmlDocPtr makeTree(SVGimage *img){

    xmlDocPtr doc = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, node = NULL;/* node pointers */
    LIBXML_TEST_VERSION;


    if(img == NULL){
        return NULL;
    }

    /* 
     * Creates a new document, a node and set it as a root node
     */
    doc = xmlNewDoc(BAD_CAST "1.0");
    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlDocSetRootElement(doc, root_node);

    xmlNewProp(root_node, BAD_CAST "xmlns", BAD_CAST img->namespace);

    if(img){

        if(strcmp(img->title, "") != 0){
            node = xmlNewChild(root_node, NULL, BAD_CAST "title", BAD_CAST img->title);
        }
        if(strcmp(img->description, "") != 0){
            node = xmlNewChild(root_node, NULL, BAD_CAST "desc", BAD_CAST img->description);
        }

        if(getLength(img->otherAttributes)!= 0){
            
            ListIterator iter = createIterator(img->otherAttributes);
            void* elem; 

            while((elem = nextElement(&iter)) != NULL){

                char name[1000];
                char value[1000];

                Attribute* atr = (Attribute*)elem;

                sprintf(name, "%s", atr->name);
                sprintf(value, "%s", atr->value);

                xmlNewProp(root_node, BAD_CAST name, BAD_CAST value);
            }
        }
    
        if(getLength(img->rectangles) != 0){

            Rectangle *rectangle;
            ListIterator riter = createIterator(img->rectangles);
            void* relem; 

            while((relem = nextElement(&riter)) != NULL){

                char x[100];
                char y[100];
                char width[100];
                char height[100];
                char units[100];

                rectangle = (Rectangle*)relem;
                node = xmlNewChild(root_node, NULL, BAD_CAST "rect", NULL);


                sprintf(units, "%s", rectangle->units);
                sprintf(x, "%0.2f", rectangle->x);
                strcat(x, units);
                sprintf(y, "%0.2f", rectangle->y);
                strcat(y, units);
                sprintf(width, "%0.2f", rectangle->width);
                strcat(width, units);
                sprintf(height, "%0.2f", rectangle->height);
                strcat(height, units);

                xmlNewProp(node, BAD_CAST "x", BAD_CAST x);
                xmlNewProp(node, BAD_CAST "y", BAD_CAST y);
                xmlNewProp(node, BAD_CAST "width", BAD_CAST width);
                xmlNewProp(node, BAD_CAST "height", BAD_CAST height);

                if(getLength(rectangle->otherAttributes)!= 0){
                    ListIterator riter2 = createIterator(rectangle->otherAttributes);
                    void* relem2; 

                    while((relem2 = nextElement(&riter2)) != NULL){

                        char name[1000];
                        char value[1000];

                        Attribute* atr = (Attribute*)relem2;

                        sprintf(name, "%s", atr->name);
                        sprintf(value, "%s", atr->value);

                        xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                    }
                }
            }
        }
        if(getLength(img->circles)!= 0){

            Circle *circle;
            ListIterator citer = createIterator(img->circles);
            void* celem; 

            while((celem = nextElement(&citer)) != NULL){

                char cx[100];
                char cy[100];
                char r[100];
                char units[100];

                circle = (Circle*)celem;
                node = xmlNewChild(root_node, NULL, BAD_CAST "circle", NULL);

                sprintf(units, "%s", circle->units);
                sprintf(cx, "%0.2f", circle->cx);
                strcat(cx, units);
                sprintf(cy, "%0.2f", circle->cy);
                strcat(cy, units);
                sprintf(r, "%0.2f", circle->r);
                strcat(r, units);

                xmlNewProp(node, BAD_CAST "cx", BAD_CAST cx);
                xmlNewProp(node, BAD_CAST "cy", BAD_CAST cy);
                xmlNewProp(node, BAD_CAST "r", BAD_CAST r);

                if(circle->otherAttributes){
                    ListIterator citer2 = createIterator(circle->otherAttributes);
                    void* celem2; 

                    while((celem2 = nextElement(&citer2)) != NULL){

                        char name[1000];
                        char value[1000];

                        Attribute* atr = (Attribute*)celem2;

                        sprintf(name, "%s", atr->name);
                        sprintf(value, "%s", atr->value);

                        xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                    }
                }
            }
        }
        if(getLength(img->paths)){

            Path *path;
            ListIterator piter = createIterator(img->paths);
            void* pelem; 

            while((pelem = nextElement(&piter)) != NULL){

                path = (Path*)pelem;
                node = xmlNewChild(root_node, NULL, BAD_CAST "path", NULL);
                
                if(getLength(path->otherAttributes) != 0){
                    ListIterator piter2 = createIterator(path->otherAttributes);
                    void* pelem2; 

                    while((pelem2 = nextElement(&piter2)) != NULL){

                        char name[1000];
                        char value[1000];

                        Attribute* atr = (Attribute*)pelem2;

                        sprintf(name, "%s", atr->name);
                        sprintf(value, "%s", atr->value);

                        xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                    }
                }
                if(path->data){
                    xmlNewProp(node, BAD_CAST "d", BAD_CAST path->data);
                }
                
            }
        }
        if(getLength(img->groups)){

            Group *group;
            ListIterator giter = createIterator(img->groups);
            void* gelem; 

            while((gelem = nextElement(&giter)) != NULL){

                group = (Group*)gelem;
                node = xmlNewChild(root_node, NULL, BAD_CAST "g", NULL);

                if(group->otherAttributes){

                    ListIterator giter2 = createIterator(group->otherAttributes);
                    void* gelem2; 

                    while((gelem2 = nextElement(&giter2)) != NULL){

                        char name[1000];
                        char value[1000];

                        Attribute* atr = (Attribute*)gelem2;

                        sprintf(name, "%s", atr->name);
                        sprintf(value, "%s", atr->value);

                        xmlNewProp(node, BAD_CAST name, BAD_CAST value);
                    }
                }

                

                makeGroup(group, node);
                
            }
        }
    }

    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();

    return doc;

}

bool validate(char* filename, char* schemaFile){

    int valid = 0;
    xmlDocPtr doc;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    xmlLineNumbersDefault(1);

    ctxt = xmlSchemaNewParserCtxt(schemaFile);

    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);
    //xmlSchemaDump(stdout, schema); //To print schema dump

    doc = xmlReadFile(filename, NULL, 0);

    if (doc == NULL){
        fprintf(stderr, "Could not parse %s\n", filename);
        valid++;
    }

    else {
    xmlSchemaValidCtxtPtr ctxt;
    int ret;

    ctxt = xmlSchemaNewValidCtxt(schema);
    xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    ret = xmlSchemaValidateDoc(ctxt, doc);

    if (ret == 0){
        valid = 0;
    }
    else if (ret > 0){
        printf("%s fails to validate\n", filename);
        valid++;
    }
    else{
        printf("%s validation generated an internal error\n", filename);
        valid++;
    }
    xmlSchemaFreeValidCtxt(ctxt);
    xmlFreeDoc(doc);
    }

    // free the resource
    if(schema != NULL)
    xmlSchemaFree(schema);

    xmlSchemaCleanupTypes();
    xmlCleanupParser();
    xmlMemoryDump();

    if(valid == 0){
        return true;
    }

    return false;
}


SVGimage* createValidSVGimage(char* fileName, char* schemaFile){

    xmlDoc *doc = xmlReadFile(fileName, NULL, 0);
    bool valid = false;
    xmlNode *root_element = NULL;
    
    /* check for empty document */
    if(doc == NULL){
        xmlFreeDoc(doc);
        return NULL;
    }
    if(fileName == NULL || schemaFile == NULL){
        xmlFreeDoc(doc);
        return NULL;
    }
    SVGimage* img = malloc(sizeof(SVGimage));

    // /*Get the root element node */
    root_element = xmlDocGetRootElement(doc);

    /* initialize lists for the image */
    img->circles = initializeList(circleToString, deleteCircle, compareCircles);
    img->paths = initializeList(pathToString, deletePath, comparePaths);
    img->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    img->groups = initializeList(groupToString, deleteGroup, compareGroups);
    img->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);


    strcpy(img->title, "");
    strcpy(img->description, "");

    xmlAttr *attr;

    for (attr = root_element->properties; attr != NULL; attr = attr->next){

        xmlNode *value = attr->children;
        char *attrName = (char *)attr->name;
        char *cont = (char *)(value->content);

        Attribute *atr = malloc(sizeof(Attribute));

        atr->name = malloc(1000);
        strcpy( atr->name, attrName);

        atr->value = malloc(1000);
        strcpy(atr->value, cont);

        insertBack(img->otherAttributes, atr);
    }

    // /* traverse the document*/
    img = traversal(root_element, img);

    valid = validateSVGimage(img, schemaFile);

    if(valid == false){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        xmlMemoryDump();
        deleteSVGimage(img);
        return NULL;
    }

    // //free libxml2 variables
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();


    return img;

}

bool writeSVGimage(SVGimage* image, char* fileName){


    if(image == NULL){
        return false;
    }

    if(validStruct(image) == false){
        return false;
    }

    if (fileName == NULL){
        return false;
    }

    xmlDocPtr doc = NULL;

    doc = makeTree(image);

    xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

    /*free the document */
    xmlFreeDoc(doc);

    return true;

}

bool validateSVGimage(SVGimage* image, char* schemaFile){

    bool valid = false;

    /* check if image is null*/
    if(image == NULL){
        return false;
    }
    /* check if values in image are false*/
    if(validStruct(image)==false){
        return false;
    }

    writeSVGimage(image, "./parser/bin/tree.svg");
    valid = validate("./parser/bin/tree.svg", schemaFile);

    if(valid == false){
        return false;
    }

    return true;

}


void setAttribute(SVGimage* image, elementType elemType, int elemIndex, Attribute* newAttribute){

    int i = 0;
    ListIterator iter;
    ListIterator aiter;
    void *aelem;
    void *elem;
    Attribute *atr;

    if(newAttribute == NULL || newAttribute->name == NULL || newAttribute->value == NULL ){
        return;
    }

    if(image == NULL){
        return;
    }
    if(validStruct(image) == false){
        return;
    }
    
    if(elemType == PATH){

        Path *path;
        iter = createIterator(image->paths);

        while((elem = nextElement(&iter)) != NULL){

            path = (Path*)elem;

            if(i == elemIndex){

                /* Edit data */
                if(strcmp("d", newAttribute->name) == 0 ){

                    free(path->data);
                    path->data = malloc(strlen(newAttribute->value) + 1000);
                    strcpy(path->data, newAttribute->value);
                    deleteAttribute(newAttribute);

                    return;
                }

                aiter = createIterator(path->otherAttributes);

                while((aelem = nextElement(&aiter)) != NULL){
                    
                    atr = (Attribute*)aelem;
                    
                    /* Edit data, Edit Attributes */
                    if(strcmp(atr->name, newAttribute->name) == 0){
                        
                        free(atr->value);
                        atr->value = malloc(strlen(newAttribute->value) + 1000);
                        strcpy(atr->value, newAttribute->value);
                        deleteAttribute(newAttribute);

                        return;
                    }
                }
                /* add data/other attributes*/
                insertBack(path->otherAttributes, newAttribute);
                return;
            }
            ++i;
        }
    }
    if(elemType == RECT){

        Rectangle *rectangle;
        iter = createIterator(image->rectangles);

        while((elem = nextElement(&iter)) != NULL){

            rectangle = (Rectangle*)elem;

            if(i == elemIndex){
                /* edit x */
                if(strcmp("x", newAttribute->name) == 0 ){
                    rectangle->x = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }
                /* edit y */
                if(strcmp("y", newAttribute->name) == 0 ){
                    rectangle->y = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }
                /* edit height*/
                if(strcmp("height", newAttribute->name) == 0 ){
                    rectangle->height = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }
                /* edit width*/
                if(strcmp("width", newAttribute->name) == 0 ){
                    rectangle->width = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }

                aiter = createIterator(rectangle->otherAttributes);

                while((aelem = nextElement(&aiter)) != NULL){
                    
                    atr = (Attribute*)aelem;
                    
                    /* Update Existing attributes */
                    if(strcmp(atr->name, newAttribute->name) == 0){
                        
                        free(atr->value);
                        atr->value = malloc(strlen(newAttribute->value) + 1000);
                        strcpy(atr->value, newAttribute->value);
                        deleteAttribute(newAttribute);

                        return;
                    }
                }
                /* Add attributes */
                insertBack(rectangle->otherAttributes, newAttribute);
                return;
            }
            ++i;   
        }
    }
    if(elemType == CIRC){

        Circle *circle;
        iter = createIterator(image->circles);

        while((elem = nextElement(&iter)) != NULL){

            circle = (Circle*)elem;

            if(i == elemIndex){
                /* Edit cx  */
                if(strcmp("cx", newAttribute->name) == 0 ){
                    circle->cx = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }
                /*edit cy*/
                if(strcmp("cy", newAttribute->name) == 0 ){
                    circle->cy = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }
                /*edit r*/
                if(strcmp("r", newAttribute->name) == 0 ){
                    circle->r = strtof(newAttribute->value, NULL);
                    deleteAttribute(newAttribute);
                    return;
                }

                aiter = createIterator(circle->otherAttributes);

                while((aelem = nextElement(&aiter)) != NULL){
                    
                    atr = (Attribute*)aelem;
                    /* update existing attributes */
                    if(strcmp(atr->name, newAttribute->name) == 0){
                        
                        free(atr->value);
                        atr->value = malloc(strlen(newAttribute->value) + 1000);
                        strcpy(atr->value, newAttribute->value);
                        deleteAttribute(newAttribute);

                        return;
                    }
                }
                /* add attributes*/
                insertBack(circle->otherAttributes, newAttribute);
                return;
            }
            ++i;
        }
    }
    if (elemType == GROUP){

        Group *group;
        iter = createIterator(image->groups);

        while((elem = nextElement(&iter)) != NULL){

            group = (Group*)elem;

            if(i == elemIndex){

                Attribute *atr;
                aiter = createIterator(group->otherAttributes);

                /* Edit existing attributes */
                while((aelem = nextElement(&aiter)) != NULL){
                    
                    atr = (Attribute*)aelem;
                    
                    if(strcmp(atr->name, newAttribute->name) == 0){
                        
                        free(atr->value);
                        atr->value = malloc(strlen(newAttribute->value) + 1000);
                        strcpy(atr->value, newAttribute->value);
                        deleteAttribute(newAttribute);
                        return;
                    }
                }
                /* add new attributes */
                insertBack(group->otherAttributes, newAttribute);
                return;
            }
            ++i;   
        }
    }
    if (elemType == SVG_IMAGE){

        Attribute *atr;
        aiter = createIterator(image->otherAttributes);

        /* edit existing attributes*/
        while((aelem = nextElement(&aiter)) != NULL){
            
            atr = (Attribute*)aelem;
            
            if(strcmp(atr->name, newAttribute->name) == 0){
                
                free(atr->value);
                atr->value = malloc(strlen(newAttribute->value) + 1000);
                strcpy(atr->value, newAttribute->value);

                deleteAttribute(newAttribute);
                return;
            }
        }
        /* add new attribute*/
        insertBack(image->otherAttributes, newAttribute);
    }
}

void addComponent(SVGimage* image, elementType type, void* newElement){

    if(image == NULL){
        return;
    }

    if(validStruct(image) == false ){
        return;
    }

    if(newElement == NULL){
        return;
    }

    if(type == RECT){
        Rectangle *rectangle = (Rectangle*)newElement;

        if(rectangle->otherAttributes == NULL){
            return;
        }

        insertBack(image->rectangles, rectangle);
    }
    if(type == CIRC){
        Circle *circle = (Circle*)newElement;

        if(circle->otherAttributes == NULL){
            return;
        }

        insertBack(image->circles, circle);
    }
    if(type == PATH){
        Path *path = (Path*)newElement;

        if(path->otherAttributes == NULL){
            return;
        }

        insertBack(image->paths, path);
    }
}


char* attrToJSON(const Attribute *a){

    char *json;
    int mallocSize = 0;

    if(a == NULL){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }

    mallocSize += strlen(a->name);
    mallocSize += strlen(a->value);

    json = malloc(mallocSize + 1000);

    sprintf(json, "{\"name\":\"%s\",\"value\":\"%s\"}", a->name, a->value);

    return json;
}
char* circleToJSON(const Circle *c){

    char *json;
    char *other;
    int mallocSize = 100000;

    if(c == NULL){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }
    other = attrListToJSON(c->otherAttributes);

    json = malloc(mallocSize);

    sprintf(json, "{\"cx\":%0.2f,\"cy\":%0.2f,\"r\":%0.2f,\"numAttr\":%d,\"units\":\"%s\"", 
    c->cx, c->cy, c->r, getLength(c->otherAttributes), c->units);

    strcat(json, ",\"Props\":");
    strcat(json, other);
    strcat(json, "}");

    return json;
}
char* rectToJSON(const Rectangle *r){

    char *json;
    char *other;
    int mallocSize = 100000;

    if(r == NULL ){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }
    other = attrListToJSON(r->otherAttributes);

    json = malloc(mallocSize);

    sprintf(json, "{\"x\":%0.2f,\"y\":%0.2f,\"w\":%0.2f,\"h\":%0.2f,\"numAttr\":%d,\"units\":\"%s\"",
    r->x, r->y, r->width, r->height, getLength(r->otherAttributes), r->units);

    strcat(json, ",\"Props\":");
    strcat(json, other);
    strcat(json, "}");

    return json;
}
char* pathToJSON(const Path *p){
    
    char *json;
    char *other;
    int mallocSize = 100000;
    char *data = malloc(65);

    if(p == NULL){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }
    other = attrListToJSON(p->otherAttributes);
    // mallocSize += strlen(p->data);
    strncpy(data, p->data, 64);
    data[64] = '\0';

    json = malloc(mallocSize);

    sprintf(json, "{\"d\":\"%s\",\"numAttr\":%d",
    data, getLength(p->otherAttributes));

    strcat(json, ",\"Props\":");
    strcat(json, other);
    strcat(json, "}");

    free(data);
    return json;
}
char* groupToJSON(const Group *g){

    char *json;
    char *other;
    int mallocSize = 100000;
    int sum = 0;

    if(g == NULL){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }
    other = attrListToJSON(g->otherAttributes);

    json = malloc(mallocSize);

    sum += getLength(g->rectangles);
    sum += getLength(g->circles);
    sum += getLength(g->paths);
    sum += getLength(g->groups);

    sprintf(json, "{\"children\":%d,\"numAttr\":%d",
    sum, getLength(g->otherAttributes));

    strcat(json, ",\"Props\":");
    strcat(json, other);
    strcat(json, "}");

    return json;
}
char* attrListToJSON(const List *list){
    
    ListIterator iter;
    ListIterator iter2;
    Attribute *atr;
    void* elem;
    char* json;
    char* string;

    if(list == NULL || getLength((void*)list) == 0){
        json = malloc(20);
        strcpy(json, "[]");
        return json;
    }

    iter = createIterator((void*)list);
    iter2 = createIterator((void*)list);
    json = malloc(100000);

    sprintf(json, "[");

    nextElement(&iter2);

    while((elem = nextElement(&iter)) != NULL){
        
        atr = (Attribute*)elem;

        string = attrToJSON(atr);

        strcat(json, string);

        if(nextElement(&iter2) != NULL){
            strcat(json, ",");
        }
        free(string);
    }
    strcat(json, "]");
    return json;
}


char* circListToJSON(const List *list){

    ListIterator iter;
    ListIterator iter2;
    Circle *circle;
    void* elem;
    char* json;
    char* string;

    if(list == NULL || getLength((void*)list) == 0){
        json = malloc(20);
        strcpy(json, "\"Circles\":[]");
        return json;
    }

    iter = createIterator((void*)list);
    iter2 = createIterator((void*)list);
    json = malloc(100000);

    sprintf(json, "\"Circles\":[");

    nextElement(&iter2);

    while((elem = nextElement(&iter)) != NULL){
        
        circle = (Circle*)elem;

        string = circleToJSON(circle);

        strcat(json, string);

        if(nextElement(&iter2) != NULL){
            strcat(json, ",");
        }
        free(string);
    }
    strcat(json, "]");
    
    return json;
}
char* rectListToJSON(const List *list){
        
    ListIterator iter;
    ListIterator iter2;
    Rectangle *rectangle;
    void* elem;
    char* string;
    char* json;

    if(list == NULL || getLength((void*)list) == 0){
        json = malloc(20);
        strcpy(json, "\"Rectangles\":[]");
        return json;
    }

    iter = createIterator((void*)list);
    iter2 = createIterator((void*)list);
    json = malloc(100000);

    sprintf(json, "\"Rectangles\":[");

    nextElement(&iter2);

    while((elem = nextElement(&iter)) != NULL){
        
        rectangle = (Rectangle*)elem;

        string = rectToJSON(rectangle);
        strcat(json, string );

        if(nextElement(&iter2) != NULL){
            strcat(json, ",");
        }
        free(string);
    }
    strcat(json, "]");
    return json;
}
char* pathListToJSON(const List *list){
    
    ListIterator iter;
    ListIterator iter2;
    Path *path;
    void* elem;
    char* json;
    char* string;

    if(list == NULL || getLength((void*)list) == 0){
        json = malloc(20);
        strcpy(json, "\"Paths\":[]");
        return json;
    }

    iter = createIterator((void*)list);
    iter2 = createIterator((void*)list);
    json = malloc(1000000);

    sprintf(json, "\"Paths\":[");

    nextElement(&iter2);

    while((elem = nextElement(&iter)) != NULL){
        
        path = (Path*)elem;

        string = pathToJSON(path);
        strcat(json, string);

        if(nextElement(&iter2) != NULL){
            strcat(json, ",");
        }

        free(string);
    }
    strcat(json, "]");
    return json;
}
char* groupListToJSON(const List *list ){
    
    ListIterator iter;
    ListIterator iter2;
    Group *group;
    void* elem;
    char* json;
    char* string;

    if(list == NULL || getLength((void*)list) == 0){
        json = malloc(20);
        strcpy(json, "\"Groups\":[]");
        return json;
    }

    iter = createIterator((void*)list);
    iter2 = createIterator((void*)list);
    json = malloc(100000);

    sprintf(json, "\"Groups\":[");

    nextElement(&iter2);

    while((elem = nextElement(&iter)) != NULL){
        
        group = (Group*)elem;

        string = groupToJSON(group);
        strcat(json, string);

        if(nextElement(&iter2) != NULL){
            strcat(json, ",");
        }
        free(string);
    }
    strcat(json, "]");
    return json;
}
char* SVGtoJSON(const SVGimage* imge, char* filename){

    int i;
    int sz;
    char *json;
    char *title;
    char *description;
    int mallocSize = 100000;
    int r, c, p, g;
    List* rects;
    List* circles;
    List* groups;
    List* paths;

    if(imge == NULL){
        json = malloc(20);
        strcpy(json, "{}");
        return json;
    }

    rects = getRects((void*)imge);
    circles = getCircles((void*)imge);
    paths = getPaths((void*)imge);
    groups = getGroups((void*)imge);

    mallocSize += 100000;
    json = malloc(mallocSize);
    
    r = getLength(rects);
    c = getLength(circles);
    p = getLength(paths);
    g = getLength(groups);

    description = malloc(500);
    title = malloc(500);

    strcpy(description, imge->description);
    strcpy(title, imge->title);

    for(i = 0; i < strlen(description); ++i){
        if(description[i] < 32 || description[i] == '\"' ){
            description[i] = ' ';
        }
    }

    for(i = 0; i < strlen(title); ++i){
        if(title[i] < 32 || title[i] == '\"'){
            title[i] = ' ';
        }
    }

    FILE *fp = fopen(filename, "r");
    fseek(fp, 0L, SEEK_END);
    float size = ftell(fp);

    sz = ceil(size) / 1000;

    sprintf(json, "{\"size\":%d,\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d,\"title\":\"%s\",\"description\":\"%s\",\"file\":\"%s\"}",sz ,r ,c, p, g, imge->title, description, filename);    
    freeList(rects);
    freeList(circles);
    freeList(groups);
    freeList(paths);

    return json;
}

SVGimage* JSONtoSVG(const char* svgString){

    if(svgString == NULL){
        return NULL;
    }
    
    SVGimage *img = malloc(sizeof(SVGimage));
    char title[1256];
    char desc[1256];

    strcpy(title, "");
    strcpy(desc, "");

    sscanf(svgString, "{\"title\":\"%[^\"]\",\"descr\":\"%[^\"]\"}", title, desc);

    img->circles = initializeList(circleToString, deleteCircle, compareCircles);
    img->paths = initializeList(pathToString, deletePath, comparePaths);
    img->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    img->groups = initializeList(groupToString, deleteGroup, compareGroups);
    img->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(img->namespace, "http://www.w3.org/2000/svg");
    strcpy(img->description, desc);
    strcpy(img->title, title);

    return img;
}
Rectangle* JSONtoRect(const char* svgString){

    if(svgString == NULL){
        return NULL;
    }

    Rectangle *rect = malloc(sizeof(Rectangle));
    char units[50];
    float x, y, width, height;
    rect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(units, "");
    x = 0;
    y = 0;
    width = 0;
    height = 0;

    sscanf(svgString, "{\"x\":%f,\"y\":%f,\"w\":%f,\"h\":%f,\"units\":\"%[^\"]\"}", &x, &y, &width, &height, units);

    units[1250] = '\0';

    rect->x = x;
    rect->y = y;
    rect->width = width;
    rect->height = height;
    strcpy(rect->units, units);

    return rect;
    
}

Circle* JSONtoCircle(const char* svgString){

    if(svgString == NULL){
        return NULL;
    }
    
    Circle *circle = malloc(sizeof(Circle));
    char units[1250];
    double cx, cy, r;
    circle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    strcpy(units, "");
    cx = 0;
    cy = 0;
    r = 0;

    sscanf(svgString, "{\"cx\":%lf,\"cy\":%lf,\"r\":%lf,\"units\":\"%[^\"]\"}", &cx, &cy, &r, units);

    units[50] = '\0';

    circle->cx = cx;
    circle->cy = cy;
    circle->r = r;
    strcpy(circle->units, units);

    return circle;

}

bool validateSVG(char* filename){

    if(validate(filename, "./parser/testFilesA2/svg.xsd") == false){
        return false;
    }

    return true;

}

/* assignment 3*/
char* getImgJson(char* filename){

    
    char* string;
    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");
    string = SVGtoJSON(img, filename);
    deleteSVGimage(img);


    return string;
}

char* dataMaker(char* filename){
    char *data = malloc(1000000);
    char *rs = malloc(1000000);
    char *cs = malloc(1000000);
    char *ps = malloc(1000000);
    char *gs = malloc(1000000);

    List* rects;
    List* circles;
    List* paths;
    List* groups;

    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");

    sprintf(data, "{");

    if(img != NULL){

        rects = getRects(img);
        circles = getCircles(img);
        paths = getPaths(img);
        groups = getGroups(img);
    }

    rs = rectListToJSON(rects);
    strcat(data, rs);
    strcat(data, ",");

    cs = circListToJSON(circles);
    strcat(data, cs);
    strcat(data, ",");

    ps = pathListToJSON(paths);
    strcat(data, ps);
    strcat(data, ",");

    gs = groupListToJSON(groups);
    strcat(data, gs);

    strcat(data, "}");

    deleteSVGimage(img);

    return data;

}

void changeTit(char* title, char* desc, char*filename){
    
    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");

    if(img == NULL){
        return;
    }
    
    img->title[0] = '\0';
    strcpy(img->title, title);

    img->description[0] = '\0';
    strcpy(img->description, desc);

    if(validateSVGimage(img, "./parser/testFilesA2/svg.xsd") == false){
        deleteSVGimage(img);
        return;
    }

    writeSVGimage(img, filename);

    deleteSVGimage(img);
}

void editAdd(char* name, char* value, char* type, int index, char* filename){
    
    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");
    Attribute* attr = malloc(sizeof(Attribute));

    attr->name = malloc(sizeof(name));
    attr->value = malloc(sizeof(value));
    strcpy(attr->name, name);
    strcpy(attr->value, value);


    if(strcmp(type, "PATH") == 0){
        setAttribute(img, PATH, index, attr);
    }
    if(strcmp(type, "RECT") == 0){
        setAttribute(img, RECT, index, attr);
    }
    if(strcmp(type, "GROUP") == 0){
        setAttribute(img, GROUP, index, attr);
    }
    if(strcmp(type, "CIRC") == 0){
        setAttribute(img, CIRC, index, attr);
    }

    if(validateSVGimage(img, "./parser/testFilesA2/svg.xsd") == false){
        deleteSVGimage(img);
        return;
    }

    writeSVGimage(img, filename);
    deleteSVGimage(img);
}

void createEmpty(char* filename){
    
    
    SVGimage* img = malloc(sizeof(SVGimage));
    Circle* circle = malloc(sizeof(Circle));
    Attribute* attr = malloc(sizeof(Attribute));


    //initialize svg
    img->circles = initializeList(circleToString, deleteCircle, compareCircles);
    img->paths = initializeList(pathToString, deletePath, comparePaths);
    img->rectangles = initializeList(rectangleToString, deleteRectangle, compareRectangles);
    img->groups = initializeList(groupToString, deleteGroup, compareGroups);
    img->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    //initialize circle
    circle->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

    circle->cx = 0;
    circle->cy = 0;
    circle->r = 45;

    attr->name = malloc(sizeof("fill"));
    attr->value = malloc(sizeof("#86eb34"));
    strcpy(attr->name,"fill");
    strcpy(attr->value,"#86eb34");



    insertBack(circle->otherAttributes, attr);
    insertBack(img->circles, circle);


    strcpy(img->title, "");
    strcpy(img->description, "");
    strcpy(img->namespace, "http://www.w3.org/2000/svg");

    writeSVGimage(img, filename);
    deleteSVGimage(img);

}

void createShape(char* filename, float x, float y, float width, float height, float r, char* fill, char* shape){

    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");
    Attribute* attr = malloc(sizeof(Attribute));


    attr->name = malloc(sizeof("fill"));
    strcpy(attr->name,"fill");

    attr->value = malloc(strlen(fill) + 1);
    strcpy(attr->value, fill);

    if(strcmp(shape, "circ") == 0){
        Circle *newCirc = malloc(sizeof(Circle));
        newCirc->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

        newCirc->cx = x;
        newCirc->cy = y;
        newCirc->r = r;
        strcpy(newCirc->units, "");

        insertBack(newCirc->otherAttributes, attr);
        insertBack(img->circles, newCirc );

        printf("%s\n", toString(img->circles));

    }
    
    if(strcmp(shape, "rect") == 0){
        Rectangle *newRect = malloc(sizeof(Rectangle));
        newRect->otherAttributes = initializeList(attributeToString, deleteAttribute, compareAttributes);

        newRect->x = x;
        newRect->y = y;
        newRect->height = height;
        newRect->width = width;
        strcpy(newRect->units, "");

        insertBack(newRect->otherAttributes, attr);
        insertBack(img->rectangles, newRect );

        printf("%s\n", toString(img->rectangles));

    }

    if(validateSVGimage(img, "./parser/testFilesA2/svg.xsd") == false){
        deleteSVGimage(img);
        return;
    }
    
    writeSVGimage(img, filename);
    deleteSVGimage(img);

}

void scaler(int x, char* shape, char* filename){

    SVGimage *img = createValidSVGimage(filename, "./parser/testFilesA2/svg.xsd");

    Circle* circle;
    Rectangle* rectangle;

    List *rectangles = getRects(img);
    List *circles = getCircles(img);

    ListIterator citer = createIterator(circles);
    ListIterator riter = createIterator(rectangles);

    if(strcmp(shape, "rect") == 0){

        void* relem;
        while((relem = nextElement(&riter)) != NULL){

            rectangle = (Rectangle*)relem;

            rectangle->width *= x;
            rectangle->height *= x;
            
        }
    }
    else{
        void* celem;
        while((celem = nextElement(&citer)) != NULL){

            circle = (Circle*)celem;
            
            circle->r *= x;
        }
    }
    
    if(validateSVGimage(img, "./parser/testFilesA2/svg.xsd") == false){
        deleteSVGimage(img);
        return;
    }

    writeSVGimage(img, filename);

    freeList(rectangles);
    freeList(circles);
    deleteSVGimage(img);
    
}

