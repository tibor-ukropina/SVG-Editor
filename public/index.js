function appendToTable(data){
    var ret = data.file.replace("./uploads/", "");
    $("#tit").append(`<tr>`);
    $("#tit").append(`<td><img id="thumbnail" src=` + data.file + ` alt="` + data.file + `"></td>`);
    $("#tit").append(`<td><a href="` + data.file + `" download>`+ ret +`</a></td>`);
    $("#tit").append(`<td scope="row">` + data.size + `KB` + `</td>`);
    $("#tit").append(`<td scope="row">` + data.numRect + `</td>`);
    $("#tit").append(`<td scope="row">` + data.numCirc + `</td>`);
    $("#tit").append(`<td scope="row">` + data.numPaths + `</td>`);
    $("#tit").append(`<td scope="row">` + data.numGroups + `</td>`);
    $("#tit").append(`</tr>`);

    //populate dropdown menu
    $("#dropdowns").append(`<option class="dropdown-item">` + ret + `</option>`);

}

function editOther(data){
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/otherAttributes',
        data: {
            test: data
        },
        success: function (data) {
            location.reload(); 
            
        },
        fail: function(error) {
            console.log(error);
        }
    });
}

function sendScale(data){
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/scaler',
        data: {
            test: data
        },
        success: function (data) {
            location.reload(); 
        },
        fail: function(error) {
            console.log(error);
        }
    });
}

function sendTitle(data){
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/test',
        data: {
            test: data
        },
        success: function (data) {
            location.reload(); 
        },
        fail: function(error) {
            console.log(error);
        }
    });
}

function makeSVG(data){
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/create',
        data: {
            test: data
        },
        success: function (data) {
            location.reload(); 
        },
        fail: function(error) {
            console.log(error);
        }
    });
}

function makeShape(data){
    
    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/shape',
        data: {
            test: data
        },
        success: function (data) {
            location.reload(); 
        },
        fail: function(error) {
            console.log(error);
        }
    });
}

// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {

    var lastOpened = "";

    $(".panelData").hide();

    $.ajax({
        type: 'get',
        dataType: 'json',
        url: '/files',
        data: {
            test: "newtest"
        },
        success: function (data) {

            console.log(data);
            
            if(data.obj.length > 0){
                $("#empty").hide();
            }
            
            for(var i = 0; i < data.obj.length; ++i){
                appendToTable(data.obj[i]);
            }

            // var string = "newTest";
            // sendTitle(string)

            $('#view-panel').append(`<img id="myimg" src="">`);
            
            $('.dropdown-item').click(function(){
                $(".panelData").show();
                $("#myimg").attr("src",$(this).val());
                $("#components").html(" ");

                for(i = 0; i < data.obj.length; ++i ){

                    var ret = data.obj[i].file.replace("./uploads/", "");

                    if(ret == $(this).val()){

                        if(data.obj[i].title == ""){
                            $("#title").html("This image has no title.");
                        }
                        else{
                            $("#title").html(data.obj[i].title);
                        }
                        if(data.obj[i].description == ""){
                            $("#desc").html("This image has no description.");
                        }
                        else{
                            $("#desc").html(data.obj[i].description);
                        }
                        // console.log(data.components[8].Rectangles[0]);
                        for(var j = 0; j < data.components[i].Rectangles.length; ++j){
                            $("#components").append(`<tr>`);
                            $("#components").append(`<td>Rectangle ` + (j + 1 ) + `</td>`);
                            $("#components").append(`<td>Upper left corner: x = ` + data.components[i].Rectangles[j].x + data.components[i].Rectangles[j].units +
                            `, y = `+ data.components[i].Rectangles[j].y + data.components[i].Rectangles[j].units +`<br>Width: `+ data.components[i].Rectangles[j].w + data.components[i].Rectangles[j].units +`
                             , Height: `+ data.components[i].Rectangles[j].h + data.components[i].Rectangles[j].units +`</td>`);
                            $("#components").append(`<td><a id="r`+j+`,`+ i +`" class="atr" data-toggle="modal" data-target="#exampleModal" href="#" onClick="return false;">` + data.components[i].Rectangles[j].numAttr + `</a></td>`);
                            $("#components").append(`</tr>`);
                        }
                        for(j = 0; j < data.components[i].Circles.length; ++j){
                            $("#components").append(`<tr>`);
                            $("#components").append(`<td>Circle ` + (j + 1 ) + `</td>`);
                            $("#components").append(`<td>Centre: x = ` + data.components[i].Circles[j].cx + data.components[i].Circles[j].units +
                            `, y = `+ data.components[i].Circles[j].cy + data.components[i].Circles[j].units +`, Radius: `+ data.components[i].Circles[j].r + data.components[i].Circles[j].units +`
                             </td>`);
                            $("#components").append(`<td><a id="c`+j+`,`+ i +`" class="atr" data-toggle="modal" data-target="#exampleModal" href="#" onClick="return false;">` + data.components[i].Circles[j].numAttr + `</a></td>`);
                            $("#components").append(`</tr>`);
                        }
                        for(j = 0; j < data.components[i].Paths.length; ++j){
                            $("#components").append(`<tr>`);
                            $("#components").append(`<td>Path ` + (j + 1 ) + `</td>`);
                            $("#components").append(`<td>path data = ` + data.components[i].Paths[j].d + `</td>`);
                            $("#components").append(`<td><a id="p`+j+`,`+ i +`" class="atr" data-toggle="modal" data-target="#exampleModal" href="#" onClick="return false;">` + data.components[i].Paths[j].numAttr + `</a></td>`);
                            $("#components").append(`</tr>`);
                        }
                        for(j = 0; j < data.components[i].Groups.length; ++j){
                            $("#components").append(`<tr>`);
                            $("#components").append(`<td>Group ` + (j + 1 ) + `</td>`);
                            $("#components").append(`<td>` + data.components[i].Groups[j].children + ` child elements</td>`);
                            $("#components").append(`<td><a id="g`+j+`,`+ i +`" class="atr" data-toggle="modal" data-target="#exampleModal" href="#" onClick="return false;">` + data.components[i].Groups[j].numAttr + `</a></td>`);
                            $("#components").append(`</tr>`);
                        }
                    }
                }

                $("#scaleShape").click(function(){

                    let array = [];
                    let f = Number($("#fct").val());
                    array.push(f);

                    if($('#rbox2').is(':checked')){
                        shape = "rect";
                    }
                    else{
                        shape = "circ";
                    }
                    array.push(shape);

                    let string3 = "./uploads/"
                    string3 += $("#myimg").attr("src");
                    array.push(string3);

                    console.log(array);
                    sendScale(array);
                
                });

                $("#submit").click(function(){

                    let array = [];
                    let string = $("#tinput").val();
                    let string2 = $("#tdesc").val();
                    
                    //send title to backend
                    $("#title").html(string);
                    array.push(string);

                    //send description to backend
                    $("#desc").html(string2);
                    array.push(string2);

                    //send file name to backend
                    let string3 = "./uploads/"
                    string3 += $("#myimg").attr("src");
                    array.push(string3);
                        
                    sendTitle(array);
                    // array.push()

                
                });

                $("#createSVG").click(function(){

                    let string = "./uploads/"
                    string += $("#fileName").val();
                    if($("#fileName").val() == ""){
                        string+= "default"
                    }
                    string += ".svg";
                        
                    makeSVG(string);
                
                });

                $("#createShape").click(function(){
                    let array = [];
                    let shape = "";
                    let filename = "./uploads/"
                    filename += $("#myimg").attr("src");
                    let x = Number($("#x").val());
                    let y = Number($("#y").val());
                    let width = Number($("#rwidth").val());
                    let height = Number($("#rheight").val());
                    let radius = Number($("#radius").val());
                    let fill = $("#daFill").val();

                    if($('#rbox').is(':checked')){
                        shape = "rect";
                    }
                    else{
                        shape = "circ";
                    }

                    array.push(filename);
                    array.push(x);
                    array.push(y);
                    array.push(width);
                    array.push(height);
                    array.push(radius);
                    array.push(fill);
                    array.push(shape);


                    console.log(array);
                    makeShape(array);
                
                });


                $('#createShape2').click(function(){
                    $('#myModal2').modal({show:true});
                })

                $('#create').click(function(){
                    $('#myModal').modal({show:true});
                })

                $('#add').click(function(){
                    $('#newShape').modal({show:true});
                })
                $('#scale').click(function(){
                    $('#scalePop').modal({show:true});
                })

                //this will send edited and new other attributes of each image
                $("#eBtn").click(function(){
                    let array = [];
                    
                    let string = $("#edit1").val();
                    let string2 = $("#edit2").val();

                    var type = lastOpened[0];
                    var index = Number(lastOpened.match(/\d+/g)[0]);

                    let string3 = "./uploads/"
                    string3 += $("#myimg").attr("src");

                    
                    if(type == 'r'){
                        type = "RECT";
                    }
                    if(type == 'c'){
                        type = "CIRC";
                    }
                    if(type == 'p'){
                        type = "PATH";
                    }
                    if(type == 'g'){
                        type = "GROUP";
                    }
                    
                    array.push(string);
                    array.push(string2);
                    array.push(type);
                    array.push(index);
                    array.push(string3);

                    console.log(array);

                    editOther(array);

                });

                $(".atr").click(function(){
                    var temp = "";
                    temp += $(this).attr("id");
                    lastOpened = $(this).attr("id");
                    
                    var type = temp[0];
                    var index = temp.match(/\d+/g)[0];
                    var index2 = temp.match(/\d+/g)[1];

                    if(type == 'r'){
                        for(var x = 0; x < data.components[index2].Rectangles[index].Props.length; ++x){
                            $("#attributes").append(`<tr>`);
                            $("#attributes").append(`<td>` + data.components[index2].Rectangles[index].Props[x].name + `</td>`);
                            $("#attributes").append(`<td>` + data.components[index2].Rectangles[index].Props[x].value + `</td>`);
                            $("#attributes").append(`</tr>`);
                        }
                    }
                    if(type == 'c'){
                        for(var x = 0; x < data.components[index2].Circles[index].Props.length; ++x){
                            $("#attributes").append(`<tr>`);
                            $("#attributes").append(`<td>` + data.components[index2].Circles[index].Props[x].name + `</td>`);
                            $("#attributes").append(`<td>` + data.components[index2].Circles[index].Props[x].value + `</td>`);
                            $("#attributes").append(`</tr>`);
                        }
                    }
                    if(type == 'p'){
                        for(var x = 0; x < data.components[index2].Paths[index].Props.length; ++x){
                            $("#attributes").append(`<tr>`);
                            $("#attributes").append(`<td>` + data.components[index2].Paths[index].Props[x].name + `</td>`);
                            $("#attributes").append(`<td>` + data.components[index2].Paths[index].Props[x].value + `</td>`);
                            $("#attributes").append(`</tr>`);
                        }
                    }
                    if(type == 'g'){
                        for(var x = 0; x < data.components[index2].Groups[index].Props.length; ++x){
                            $("#attributes").append(`<tr>`);
                            $("#attributes").append(`<td>` + data.components[index2].Groups[index].Props[x].name + `</td>`);
                            $("#attributes").append(`<td>` + data.components[index2].Groups[index].Props[x].value + `</td>`);
                            $("#attributes").append(`</tr>`);
                        }
                    }
                });

                $("#cBtn").click(function(){
                    $("#attributes").html("");
                });
            });
            
        },
        fail: function(error) {
            console.log(error);
        }
    });
});