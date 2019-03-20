// Put all onload AJAX calls here, and event listeners
$(document).ready(function() {
    // On page-load AJAX Example
    $.ajax({
        type: 'get',            //Request type
        dataType: 'json',       //Data type - we will use JSON for almost everything 
        url: '/someendpoint',   //The server endpoint we are connecting to
        success: function (data) {
            /*  Do something with returned object
                Note that what we get is an object, not a string, 
                so we do not need to parse it on the server.
                JavaScript really does handle JSONs seamlessly
            */
            $('#blah').html("On page load, Received string '"+JSON.stringify(data)+"' from server");
            //We write the object to the console to show that the request was successful
            console.log(data); 

        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    $.ajax({
        type:'get',
        dataType:'json',
        url:'/files',
        success: function(data){
            console.log("firstData:"+data);
            if(data.length != 0){
                for (index = 0; index < data.length; index++) { 
                    let name = data[index];
                    nameSender(name);
                } 
            }else{
                $('#calendar_list > tbody').append("<tr><td>No Files Found</td></tr>");
            }
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });

    

    // Event listener form replacement example, building a Single-Page-App, no redirects if possible
    $('#someform').submit(function(e){
        $('#blah').html("Callback from the form");
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });

    $('#statusBox').submit(function(e){
        $('#clear').value="Callback from the form";
        document.getElementById("status").value = "";
        e.preventDefault();
        //Pass data to the Ajax call, so it gets passed to the 
        $.ajax({});
    });


});

function selectedFile(){

    let dropdown = document.getElementById('calendar_select');
    let selected = dropdown.options[dropdown.selectedIndex].text;
    console.log("On change: "+selected);
    $.ajax({
        type:'get',
        dataType:'json',
        data: {filename: selected},
        url:'/inspectCal',
        success: function(data){
            console.log("file success!");
            //console.log(name+3);
            $('#calendar_contents > tbody').remove();
            $('#event_select').empty();

            for (index = 0; index < data.length; index++) { 
                let cal = data[index];
                cal.startDT.date = cal.startDT.date.slice(0, 4) + "/" + cal.startDT.date.slice(4, 6)+"/" + cal.startDT.date.slice(6);
                cal.startDT.time = cal.startDT.time.slice(0, 2) + ":" + cal.startDT.time.slice(2, 4)+":" + cal.startDT.time.slice(4);
                if(cal.startDT.isUTC == true){
                    cal.startDT.time+=" (UTC)";
                }
                $('#calendar_contents').append("<tbody><tr><td>"+index+"</td><td>"+cal.startDT.date+"</td><td>"+cal.startDT.time+"</td><td>"+cal.summary+"</td><td>"+cal.numProps+"</td><td>"+cal.numAlarms+"</td></tr></tbody>");
                $('#event_select').append("<option value="+index+">"+index+"</option>");
            } 


        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
}

function nameSender(name){
    $.ajax({
        type:'get',
        dataType:'json',
        data: {filename: name},
        url:'/getcals',
        success: function(data2){
            //console.log(name+3);
            console.log(data2.calJSON)
            console.log(data2.calJSON.filename)
            if(data2.calJSON.version != undefined){
                $('#calendar_list > tbody').append("<tr><td><a href=\"/uploads/"+data2.calJSON.filename+"\">"+data2.calJSON.filename+"</a></td><td>"+data2.calJSON.version+"</td><td>"+data2.calJSON.prodID+"</td><td>"+data2.calJSON.numEvents+"</td><td>"+data2.calJSON.numProps+"</td></tr>");
                $('#calendar_select').append("<option value="+data2.calJSON.filename+">"+data2.calJSON.filename+"</option>");
            
            }//console.log(name+3.5 );
            selectedFile();
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
}

function scrollBot(){
    let textarea = document.getElementById('status');
    textarea.scrollTop = textarea.scrollHeight;
}

function validator(){
    let filename = document.getElementById("file_selector").value;
    if(filename == undefined){
        document.getElementById("status").value += "Cannot upload non ics file.\n";
    }
    let suffix = filename.substring(filename.length-4);
    if(suffix != ".ics"){
        document.getElementById("upload_cal").disabled = true;
        document.getElementById("status").value += "Cannot upload non ics file.\n";
    }else{
        document.getElementById("upload_cal").disabled = false;
    }
    scrollBot();
}

function addEvent() {
    let formData = document.getElementById("event_form");
    if(formData.startdate.value.length !=10){
        document.getElementById("status").value += "Event start date is invalid\n";
        scrollBot();
        return;
    }
    if(formData.starttime.value.length !=8){
        document.getElementById("status").value += "Event start time is invalid\n";
        scrollBot();
        return;
    }

    if(formData.crdate.value.length !=10){
        document.getElementById("status").value += "Event creation date is invalid\n";
        scrollBot();
        return;
    }
    if(formData.crtime.value.length !=8){
        document.getElementById("status").value += "Event creation time is invalid\n";
        scrollBot();
        return;
    }

    if(formData.UID.value.length ==0){
        document.getElementById("status").value += "UID must not be empty\n";
        scrollBot();
        return;
    }
    
    let evtJSON = {
        UID: formData.UID.value,
        creationDT: {
            date: formData.crdate.value.slice(0,4)+formData.crdate.value.slice(5,7)+formData.crdate.value.slice(8),
            time: formData.crtime.value.slice(0,2)+formData.crtime.value.slice(3,5)+formData.crtime.value.slice(6),
            isUTC: formData.CUTC.checked
        },
        startDT: {
            date: formData.startdate.value.slice(0,4)+formData.startdate.value.slice(5,7)+formData.startdate.value.slice(8),
            time: formData.starttime.value.slice(0,2)+formData.starttime.value.slice(3,5)+formData.starttime.value.slice(6),
            isUTC: formData.SUTC.checked
        },
        summary: formData.summary.value,
        numProps: 0,
        numAlarms: 0
    };
    console.log(JSON.stringify(evtJSON));
    let dropdown = document.getElementById('calendar_select');
    let selected = dropdown.options[dropdown.selectedIndex].text;
    scrollBot();
    $.ajax({
        type:'get',
        data: {filename: selected, event: JSON.stringify(evtJSON)},
        dataType:'json',
        url:'/addEvent',
        success: function(data2){
            let dat = data2.calJSON;
            if(dat.error!=undefined&&dat.error!="Success"){
                
                document.getElementById("status").value += dat.error+"\n";
                scrollBot();
                return;
            }
            $.ajax({
                type:'get',
                dataType:'json',
                url:'/files',
                success: function(data){
                    $('#calendar_list > tbody').empty();
                    $('#calendar_select').empty();
                    if(data.length != 0){
                        for (index = 0; index < data.length; index++) { 
                            let name = data[index];
                            nameSender(name);
                        }
                        document.getElementById('status').value += "Event Added to calendar " + selected + ".\n"; 
                    }else{
                        $('#calendar_list > tbody').append("<tr><td>No Files Found</td></tr>");
                    }
                    scrollBot();
                    formData.startdate.value = "";
                    formData.starttime.value = "";
                    formData.crdate.value = "";
                    formData.crtime.value = "";
                    formData.UID.value = "";
                    formData.summary.value = "";
                    formData.SUTC.checked = false;
                    formData.CUTC.checked = false;
                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error); 
                }
            });
        },
        fail: function() {

        }
    });
}

function addCal() {

    let formData = document.getElementById("cal_form");
    let suffix = formData.filename.value.substring(formData.filename.value.length-4);
    if(suffix != ".ics"){
        document.getElementById("status").value += "Cannot create non ics file.(please end the name with .ics)\n";
        scrollBot();
        return;
    }

    if(Number.isInteger(formData.version.value)||parseInt(formData.version.value)<=0||isNaN(parseInt(formData.version.value))){
        document.getElementById("status").value += "Calendar version is invalid. (must be positive integer)\n";
        scrollBot();
        return;
    }
    if(formData.prodID.value.length == 0){
        document.getElementById("status").value += "Calendar product id is invalid\n";
        scrollBot();
        return;
    }

    if(formData.ncstartdate.value.length !=10){
        document.getElementById("status").value += "Event start date is invalid\n";
        scrollBot();
        return;
    }
    if(formData.ncstarttime.value.length !=8){
        document.getElementById("status").value += "Event start time is invalid\n";
        scrollBot();
        return;
    }

    if(formData.nccrdate.value.length !=10){
        document.getElementById("status").value += "Event creation date is invalid\n";
        scrollBot();
        return;
    }
    if(formData.nccrtime.value.length !=8){
        document.getElementById("status").value += "Event creation time is invalid\n";
        scrollBot();
        return;
    }

    if(formData.ncUID.value.length ==0){
        document.getElementById("status").value += "UID must not be empty\n";
        scrollBot();
        return;
    }
    let calJSON = {
        version: parseInt(formData.version.value),
        prodID: formData.prodID.value,
        evtJSON: {
            UID: formData.ncUID.value,
            creationDT: {
                date: formData.nccrdate.value.slice(0,4)+formData.nccrdate.value.slice(5,7)+formData.nccrdate.value.slice(8),
                time: formData.nccrtime.value.slice(0,2)+formData.nccrtime.value.slice(3,5)+formData.nccrtime.value.slice(6),
                isUTC: formData.ncCUTC.checked
            },
            startDT: {
                date: formData.ncstartdate.value.slice(0,4)+formData.ncstartdate.value.slice(5,7)+formData.ncstartdate.value.slice(8),
                time: formData.ncstarttime.value.slice(0,2)+formData.ncstarttime.value.slice(3,5)+formData.ncstarttime.value.slice(6),
                isUTC: formData.ncSUTC.checked
            },
            summary: formData.ncsummary.value,
            numProps: 0,
            numEvents: 0
        }
    };
    console.log(JSON.stringify(calJSON));

    scrollBot();
    $.ajax({
        type:'get',
        data: {filename: formData.filename.value, calendar: JSON.stringify(calJSON)},
        dataType:'json',
        url:'/addCal',
        success: function(data2){
            let dat = data2.calJSON;
            if(dat.error!=undefined&&dat.error!="Success"){
                
                document.getElementById("status").value += dat.error+"\n";
                scrollBot();
                return;
            }
            $.ajax({
                type:'get',
                dataType:'json',
                url:'/files',
                success: function(data){
                    console.log("Got Here");
                    $('#calendar_list > tbody').empty();
                    $('#calendar_select').empty();
                    if(data.length != 0){
                        for (index = 0; index < data.length; index++) { 
                            let name = data[index];
                            nameSender(name);
                        }
                        document.getElementById('status').value += "Calendar named " + formData.filename.value + " created.\n"; 
                    }else{
                        $('#calendar_list > tbody').append("<tr><td>No Files Found</td></tr>");
                    }
                    scrollBot();
                    formData.filename.value = "";
                    formData.version.value = "";
                    formData.prodID.value = "";
                    formData.ncstartdate.value = "";
                    formData.ncstarttime.value = "";
                    formData.nccrdate.value = "";
                    formData.nccrtime.value = "";
                    formData.ncUID.value = "";
                    formData.ncsummary.value = "";
                    formData.ncSUTC.checked = false;
                    formData.ncCUTC.checked = false;

                },
                fail: function(error) {
                    // Non-200 return, do something with error
                    console.log(error); 
                }
            });


        },
        fail: function() {

        }
    });
}

function escapeSpecialChars(jsonString) {

    return jsonString.replace(/\n/g, "\\n")
        .replace(/\r/g, "\\r")
        .replace(/\t/g, "\\t")
        .replace(/\f/g, "\\f");

}

function showAlarms(){
    let caldropdown = document.getElementById('calendar_select');
    let calselected = caldropdown.options[caldropdown.selectedIndex].text;
    let evtdropdown = document.getElementById('event_select');
    let num = parseInt(evtdropdown.options[evtdropdown.selectedIndex].text);
    document.getElementById("status").value += "Showing Alarms for event # " + num + " in calendar " + calselected + "\n---------------------------------------------------------------------------\n";
    $.ajax({
        type:'get',
        dataType:'json',
        data: {filename: calselected, evtNum: num},
        url:'/almList',
        success: function(data){
            let dat = JSON.parse(escapeSpecialChars(data.almListJSON));
            if(dat.error!=undefined&&dat.error!="Success"){
                document.getElementById("status").value += dat.error+"\n";
                scrollBot();
                return;
            }
            
            if(dat.length != 0){
                console.log(dat);
                for (index = 0; index < dat.length; index++) { 
                    console.log("getting alarms");
                    document.getElementById("status").value += "Alarm # " + index + "\n------------------------------\n" + dat[index].slice(14, dat[index].length-12) + "----------------------------\n";
                } 
            }else{
                document.getElementById("status").value += "No alarms in this event.\n";
            }
            scrollBot();
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
}

function showProps(){
    let caldropdown = document.getElementById('calendar_select');
    let calselected = caldropdown.options[caldropdown.selectedIndex].text;
    let evtdropdown = document.getElementById('event_select');
    let num = parseInt(evtdropdown.options[evtdropdown.selectedIndex].text);
    document.getElementById("status").value += "Showing Optional Properties for event # " + num + " in calendar " + calselected + "\n---------------------------------------------------------------------------\n";
    $.ajax({
        type:'get',
        dataType:'json',
        data: {filename: calselected, evtNum: num},
        url:'/propList',
        success: function(data){
            let dat = JSON.parse(escapeSpecialChars(data.propListJSON));
            if(dat.error!=undefined&&dat.error!="Success"){
                document.getElementById("status").value += dat.error+"\n";
                scrollBot();
                return;
            }
            
            if(dat.length != 0){
                console.log(dat);
                for (index = 0; index < dat.length; index++) { 
                    console.log("getting props");
                    document.getElementById("status").value += "Optional Property # " + index + "\n------------------------------\n" + dat[index] + "----------------------------\n";
                } 
            }else{
                document.getElementById("status").value += "No optional properties in this event.\n";
            }
            scrollBot();
        },
        fail: function(error) {
            // Non-200 return, do something with error
            console.log(error); 
        }
    });
}


