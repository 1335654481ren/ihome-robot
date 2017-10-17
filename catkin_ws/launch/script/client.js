/**
 * Created with JetBrains WebStorm.
 * User: Administrator
 * Date: 12-10-26
 * Time: 下午3:56
 * To change this template use File | Settings | File Templates.
 */

var net = require('net');
var port = 10001;
var host = '127.0.0.1';

var client= new net.Socket();
client.setEncoding('binary');
//连接到服务端
client.connect(port,host,function(){

    client.write("任晓亮，你好");

});

client.on('data',function(data){
    console.log('recv data:'+ data);

});
client.on('error',function(error){

    console.log('error:'+error);
    client.destory();

});
client.on('close',function(){

    console.log('Connection closed');


});
