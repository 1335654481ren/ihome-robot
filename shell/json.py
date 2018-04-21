import json

f = open("a.json", "rb")
data = json.load(f)
#print data
image_width = data['camera']['image_width']
print "image_width = " + str(image_width)
cls_mode = data['cls_mode']   
print "cls_mode = " + cls_mode
data['cls_mode'] = "rnn"
fd = open("b.json", 'wb')
fd.write(json.dumps(data,indent=2))
fd.close()


