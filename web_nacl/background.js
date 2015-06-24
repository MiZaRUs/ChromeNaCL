//  Copyright (C) 2015 by Oleg Shirokov   olgshir@gmail.com
chrome.extension.onMessage.addListener(function(request, sender, func_callback) {
    if( request.cmd == "init"){	// получить IP обекта и дополнительные атрибуты передать в главную страницу
        console.log("Ищем IP "+ request.h);	// можно использовать нативные функции
//        chrome.extension.sendNativeMessage('com.my.get_ip', { cmd: request.h }, function( respon ){
//            console.log('sendNativeMessage respon ',  respon);
            chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
                chrome.tabs.sendMessage(tabs[0].id, {res: "SetIP", d: "192.168.1.1" }, function(response) {  });
            });
//        });
    }// init
//  --
    if (request.cmd == "XFreeRDP"){	// запуск xfreerdp с полученным IP. укажите пользователя и пароль для доступа к удаленному серверу
        console.log("Ждём XFreeRDP "+ request.h);
        chrome.extension.sendNativeMessage('com.my.exec', { cmd: "xfreerdp /u:user /p:passwd /size:1916x1060 /bpp:16 /compression /cert-ignore +clipboard +fonts /drive:Public,/home/Public /v:"+request.h }, function( respon ){
            console.log('sendNativeMessage respon ',  respon);
            chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
                chrome.tabs.sendMessage(tabs[0].id, {res: "Exec", d: respon }, function(response) {  });
            });
        });
        func_callback( "XFreeRDP Ok" );
    }
//  --
    if (request.cmd == "SysLog"){	// получение основных сообщений из системного журнала винды по SSH по публичному ключу
        console.log("Ждём WinSysLog "+ request.h);
        chrome.extension.sendNativeMessage('com.my.ssh_cli', { cmd: "user@"+request.h+" -i ~/.ssh/pub_rsa "+"psloglist /accepteula -i 19,41,109,6008,7000,7022,7038,1074 -d 2 | iconv -f CP1251 -t UTF-8" }, function( respon ){
            console.log('sendNativeMessage respon ',  respon);
            chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
                chrome.tabs.sendMessage(tabs[0].id, {res: "ShowTXT", d: respon }, function(response) {  });
            });
        });
        func_callback("SysLog Ok" );
    }
//  --
    if (request.cmd == "GitStatus"){	// Получение подготовленного файла с отчетом git status по SSH по публичному ключу
        console.log("Ждём GitStatus "+ request.h);
        chrome.extension.sendNativeMessage('com.my.ssh_cli', { cmd: "user@"+request.h+" -i ~/.ssh/pub_rsa "+"cat /cygdrive/d/imus/monitoring/SMonitor/log/git-status.log | iconv -f CP1251 -t UTF-8" }, function( respon ){
            console.log('sendNativeMessage respon ',  respon);
            chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
                chrome.tabs.sendMessage(tabs[0].id, {res: "ShowTXT", d: respon }, function(response) {  });
            });
        });

        func_callback( "Git Status Ok" ); //обратное сообщение
    }
//  --
    if (request.cmd == "QueuesAMQ"){	// Получение xml отчёта и преобразование в табличное представление
        console.log("Ждём QueuesAMQ "+ request.h +" : "+request.o);
        port = "8161";	// получаем с доп.параметрами
        chrome.extension.sendNativeMessage('com.my.ssh_cli', { cmd: "user@"+request.h+" -i ~/.ssh/pub_rsa "+"curl http://localhost:"+port+"/admin/xml/queues.jsp" }, function( respon ){
            res = respon.replace(/<br>/g, "" );
            if( res.length > 20 ){
// распарсим XML
                if( window.DOMParser ){
                    parser=new DOMParser();
                    xmlDoc=parser.parseFromString( res, "text/xml" );
                    res_str  = "Очереди, размер которых более 3:<br>";
                    res_str += "<table width='100%' cellspacing='0' cellpadding='1' border='2'";
                    for( i = 0; i < xmlDoc.getElementsByTagName("queue").length; i++ ){	// length_list
                        if( xmlDoc.getElementsByTagName("queue")[i].childNodes[1].attributes[0].value > 3 ){	//size
                            res_str += "<tr><td style='color: black; text-align: left;'>";
                            res_str += xmlDoc.getElementsByTagName("queue")[i].attributes[0].value;//queue_name
                            res_str += "</td><td style='color: blue; text-align: right;'>";
                            res_str += xmlDoc.getElementsByTagName("queue")[i].childNodes[1].attributes[0].value;	//size
                            res_str += "</td><td style='color: blue; text-align: right;'>";
                            res_str += xmlDoc.getElementsByTagName("queue")[i].childNodes[1].attributes[1].value;	//cons
                            res_str += "</td><td style='color: black; text-align: right;'>";
                            res_str += xmlDoc.getElementsByTagName("queue")[i].childNodes[1].attributes[2].value;	// do
                            res_str += "</td><td style='color: black; text-align: right;'>";
                            res_str += xmlDoc.getElementsByTagName("queue")[i].childNodes[1].attributes[3].value;
                            res_str += "</td></tr>";
                        }
                    }
                    res_str += "</table>";
                    console.log('XML respon ', res_str );
                }
//  --
                chrome.tabs.query({active: true, currentWindow: true}, function(tabs) {
                    chrome.tabs.sendMessage(tabs[0].id, {res: "ShowTXT", d: res_str }, function(response) {  });
                });
            }
        });

        func_callback( "Queues AMQ Ok" ); //обратное сообщение
    }
//  --
});
