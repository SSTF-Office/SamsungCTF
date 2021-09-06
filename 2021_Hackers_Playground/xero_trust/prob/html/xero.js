var next_list_idx;

// Prototypes
ArrayBuffer.prototype.toUint8Array = Array.prototype.toUint8Array = function () { return new Uint8Array(this); }
Uint32Array.prototype.toUint8Array = function () { return new Uint8Array(this.buffer); }
Uint8Array.prototype.concat = function(a) { return new Uint8Array(Array.from(this).concat(Array.from(a))); }
Uint8Array.prototype.decode = function() { return String.fromCharCode.apply(null, this); }
Uint8Array.prototype.decodeutf8 = function() { return (new TextDecoder("utf-8")).decode(this); }
Uint8Array.prototype.toArray = function() { return  Array.from(this); }
String.prototype.encode = function () { return new Uint8Array(Array.prototype.map.call(this, x=>x.charCodeAt())); }
String.prototype.encodeutf8 = function () { return (new TextEncoder("utf-8")).encode(this); }

// Page initialization functions
function init_index() {
	check_id();
	var mate_id = sessionStorage.getItem("mate");
	document.getElementById("mate_id").innerHTML = "<a href=./write.html#"+mate_id+">"+mate_id+"</a>";
	refresh_msg();
}

function init_write() {
	check_id();
	document.msgform.to.value = window.location.hash.replace("#", "");
}

function init_enter() {
	var id = sessionStorage.getItem("id");
	var key = sessionStorage.getItem("key");
	if (id && key) {
		location.href="./";
	} else {
		document.enter.id.value = crypto.getRandomValues(new Uint32Array(1))[0] & 0x7FFFFFFF;
	}
}

function check_id() {
	var id = sessionStorage.getItem("id");
	var key = sessionStorage.getItem("key");
	if (!id || !key) {
		location.href="./enter.html";
	}
	document.getElementById("id").innerHTML = id;
}

// Request functions

async function request_sendmsg(payload) {
	var res = await fetch("./sendmsg.php?msg="+encodeURIComponent(payload));
	return res.json();
}

async function request_getmsg(id, list_idx=0, order="desc") {
	var result = await fetch("./getmsg.php?list="+list_idx+"&id="+id+"&order="+order).then(res=>res.json());
	return result;
}

async function request_getkey(id) {
	var result = await fetch("./getkey.php?id="+id).then(res=>res.json());
	return result;
}

// Functions

async function more_msg() {
	refresh_msg(++next_list_idx);
}

async function refresh_msg(list_idx=0) {
	if (list_idx == 0) next_list_idx = 0;
	var id = sessionStorage.getItem("id");
	var key = await import_key(sessionStorage.getItem("key").encode());
	var result = await request_getmsg(id, list_idx);
	var resultHTML = "<table><tr><th>idx</th><th>from</th><th>to</th><th>msg</th></tr>";
	for(var msg of result) {
		var plain_msg = await decrypt_msg(key, msg.msg);
		var msg32 = new Uint32Array(plain_msg.encode().buffer.slice(0, 8));
		var from = msg32[0];
		var to = msg32[1];
		resultHTML += "<tr><td>"+msg.idx+"</td><td><a href=write.html#"+from+">"+from+"</a></td><td>"+to+"</td><td>"+JSON.parse(plain_msg.slice(8)).msg.replaceAll("<", "&lt;").replaceAll(">", "&gt;")+"</td></tr>";
	}
	resultHTML += "</table>";
	document.getElementById("msgs").innerHTML = resultHTML;
}

async function pack_msg(from, to, msg) {
	var key = await import_key(sessionStorage.getItem("key").encode());
	var plain = new Uint32Array([from, to]).toUint8Array().concat(JSON.stringify({"msg":msg}).encodeutf8());
	var encrypted_msg = await encrypt_msg(key, plain);
	return encrypted_msg;
}

async function send_msg() {
	var msg = document.msgform.msg.value;
	var to = document.msgform.to.value;
	var from = sessionStorage.getItem("id");
	var encrypted_msg = await pack_msg(from, to, msg);
	var result = await send_msg_raw(from, 0, encrypted_msg);
	if (result.result == "no") {
		alert("Key is wrong. Get a new ID.");
		reset_id();
	} else if (result.result == "filter") {
		alert("Your message is not transmitted.");
	} else if (result.result == "flag") {
		alert("FLAG is "+result.flag);
	} else {
		location.href="./index.html";
	}
}

async function send_msg_raw(from, to, msg) {
	var payload = btoa(new Uint32Array([from, to]).toUint8Array().concat(msg).decode());
	return await request_sendmsg(payload);
}

function reset_id() {
	sessionStorage.removeItem("id");
	sessionStorage.removeItem("key");
	location.href="./";
}

async function get_key() {
	var id = document.enter.id.value;
	var result = await request_getkey(id);
	if (result.result == "no") {
		alert("Choose another ID");
	} else {
		sessionStorage.setItem("id", id);
		sessionStorage.setItem("key", atob(result.key));
		sessionStorage.setItem("mate", result.mate);
		location.href="./";
	}
}

// Cryptography functions
async function import_key(key) {
	return await crypto.subtle.importKey("raw", key, "AES-CBC", false, ["encrypt", "decrypt"]);
}

async function encrypt_msg(key, msg) {
	let encrypted = new Array();
	let arr = msg;
	for(let i = 0; i <= Math.floor(arr.length / 15); i++) {
		let enc_block = await crypto.subtle.encrypt({name: "AES-CBC", iv: new Uint8Array(16)}, key, arr.slice(i * 15, (i + 1) * 15));
		encrypted = encrypted.concat(enc_block.toUint8Array().toArray());
	}
	return encrypted.toUint8Array();
}

async function decrypt_msg(key, msg) {
	let decrypted = new Array();
	let arr = atob(msg).encode();
	for(let i = 0; i < Math.ceil(arr.length / 16); i++) {
		let dec_block = await crypto.subtle.decrypt({name: "AES-CBC", iv: new Uint8Array(16)}, key, arr.slice(i * 16, (i + 1) * 16));
		decrypted = decrypted.concat(dec_block.toUint8Array().toArray());
	}
	return decrypted.toUint8Array().decodeutf8();
}

