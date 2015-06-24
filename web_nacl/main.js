//  Copyright (C) 2015 by Oleg Shirokov   olgshir@gmail.com
var obj_ip=null;
var obj_code=null;
document.addEventListener("DOMContentLoaded", function(){
    obj_code=document.getElementById('summary-val').innerHTML;
//  --
    var dm=document.getElementById('viewissuesidebar');
    if(dm != null){
        dm.innerHTML += "<br><h4 id='IP'></h4><br>";
//  --
        dm.innerHTML += "<button id='sm_bt_RDP' style='width:300px;height:22px'>RDP</button><br>";
        dm.innerHTML += "<button id='sm_bt_WSL' style='width:300px;height:22px'>Просмотр системного журнала</button><br>";
        dm.innerHTML += "<button id='sm_bt_GIT' style='width:300px;height:22px'>Просмотр статуса Git </button><br>";
        dm.innerHTML += "<h6 id='SM_TXT'></h6>";
//  --
        document.getElementById("sm_bt_RDP").addEventListener("click", function(){
            buttonExec( "XFreeRDP" );
        });
//  --
        document.getElementById("sm_bt_WSL").addEventListener("click", function(){
            buttonExec( "SysLog" );
        });
//  --
        document.getElementById("sm_bt_GIT").addEventListener("click", function(){
            buttonExec( "GitStatus" );
        });
//  --
        chrome.extension.sendMessage({cmd: "init", h: obj_code }, function(backMessage){
            console.log('Обратно принято из фона:', backMessage);
        });
//  -------
    }// if dm
//  ---------
    chrome.runtime.onMessage.addListener(function(req, sender, func_callback) {
        console.log('Message:', req);
        if( req.res == "SetIP") {
            console.log("Show IP: "+ req.d );
            obj_ip = req.d;
            document.getElementById('IP').innerHTML = "IP: " + obj_ip;
        }else if( req.res == "ShowTXT"){
            console.log("Show TXT: "+ req.d );
            document.getElementById('SM_TXT').innerHTML = req.d;
        }else if( req.res == "Exec"){
            console.log("Exec: ", req.d);
            document.getElementById('SM_TXT').innerHTML = req.d;
        }else{
            off_wait();
        }
    });
});
//  ------------------------
function buttonExec( bcmd ){
    console.log( "ButtonExec: ", bcmd+" : "+obj_ip+" : "+obj_code );
    if(( bcmd != null )&&( obj_ip != null )&&( obj_code != null )){
        chrome.extension.sendMessage({cmd: bcmd, h: obj_ip, o: obj_code }, function(backMessage){
            console.log('Принято из фона:', backMessage);
        });
        on_wait();
    }else{
        document.getElementById('SM_TXT').innerHTML = "Не достаточно параметров для выполнения!";
    }
}
//  --
function on_wait(){
    document.getElementById('SM_TXT').innerHTML = "Команда отправлена. Ждём.";
}
//  --
function off_wait(){
    document.getElementById('SM_TXT').innerHTML = "?!";
}
