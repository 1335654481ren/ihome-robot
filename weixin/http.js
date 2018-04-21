var http = require('http');
　　http.createServer(function (req, res) {
//	  console.log(req);
	  console.log("get request");
　　  res.writeHead(200, {'Content-Type': 'text/plain'});
 　　 res.end('Hello World from renxl\n');
　　}).listen(18080);
　　console.log('Server running at http://127.0.0.1:18080/');
