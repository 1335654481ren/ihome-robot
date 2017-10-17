// Auto-generated. Do not edit!

// (in-package robot_msgs.srv)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------


//-----------------------------------------------------------

class SpeakRequest {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.id = null;
      this.type = null;
      this.arg1 = null;
      this.arg2 = null;
    }
    else {
      if (initObj.hasOwnProperty('id')) {
        this.id = initObj.id
      }
      else {
        this.id = 0;
      }
      if (initObj.hasOwnProperty('type')) {
        this.type = initObj.type
      }
      else {
        this.type = 0;
      }
      if (initObj.hasOwnProperty('arg1')) {
        this.arg1 = initObj.arg1
      }
      else {
        this.arg1 = '';
      }
      if (initObj.hasOwnProperty('arg2')) {
        this.arg2 = initObj.arg2
      }
      else {
        this.arg2 = '';
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type SpeakRequest
    // Serialize message field [id]
    bufferOffset = _serializer.uint8(obj.id, buffer, bufferOffset);
    // Serialize message field [type]
    bufferOffset = _serializer.uint8(obj.type, buffer, bufferOffset);
    // Serialize message field [arg1]
    bufferOffset = _serializer.string(obj.arg1, buffer, bufferOffset);
    // Serialize message field [arg2]
    bufferOffset = _serializer.string(obj.arg2, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type SpeakRequest
    let len;
    let data = new SpeakRequest(null);
    // Deserialize message field [id]
    data.id = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [type]
    data.type = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [arg1]
    data.arg1 = _deserializer.string(buffer, bufferOffset);
    // Deserialize message field [arg2]
    data.arg2 = _deserializer.string(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += object.arg1.length;
    length += object.arg2.length;
    return length + 10;
  }

  static datatype() {
    // Returns string type for a service object
    return 'robot_msgs/SpeakRequest';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'e65633c461c2612af19a09955e426447';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint8 id
    uint8 type
    string arg1
    string arg2
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new SpeakRequest(null);
    if (msg.id !== undefined) {
      resolved.id = msg.id;
    }
    else {
      resolved.id = 0
    }

    if (msg.type !== undefined) {
      resolved.type = msg.type;
    }
    else {
      resolved.type = 0
    }

    if (msg.arg1 !== undefined) {
      resolved.arg1 = msg.arg1;
    }
    else {
      resolved.arg1 = ''
    }

    if (msg.arg2 !== undefined) {
      resolved.arg2 = msg.arg2;
    }
    else {
      resolved.arg2 = ''
    }

    return resolved;
    }
};

class SpeakResponse {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.code = null;
    }
    else {
      if (initObj.hasOwnProperty('code')) {
        this.code = initObj.code
      }
      else {
        this.code = 0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type SpeakResponse
    // Serialize message field [code]
    bufferOffset = _serializer.uint32(obj.code, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type SpeakResponse
    let len;
    let data = new SpeakResponse(null);
    // Deserialize message field [code]
    data.code = _deserializer.uint32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 4;
  }

  static datatype() {
    // Returns string type for a service object
    return 'robot_msgs/SpeakResponse';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '7d996991b911ad3473a76f58f16c28fc';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    uint32 code
    
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new SpeakResponse(null);
    if (msg.code !== undefined) {
      resolved.code = msg.code;
    }
    else {
      resolved.code = 0
    }

    return resolved;
    }
};

module.exports = {
  Request: SpeakRequest,
  Response: SpeakResponse,
  md5sum() { return 'ffe2ed0131a0c7d14cc080adec6a4104'; },
  datatype() { return 'robot_msgs/Speak'; }
};
