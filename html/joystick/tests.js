function testRandom() {
	var target_x = 0;
	var target_y = 0;
	var target_x_cur = 0;
	var target_y_cur = 0;

	function mouseEvent(node, type, sx, sy, cb, cb_param, delay) {
		var evt = document.createEvent ('MouseEvents');
		var e = {
			bubbles: true,
			cancelable: (type != "mousemove"),
			view: window,
			detail: 0,
			screenX: sx + offset_x, 
			screenY: sy + offset_y,
			clientX: sx + offset_x,
			clientY: sy + offset_y,
			ctrlKey: false,
			altKey: false,
			shiftKey: false,
			metaKey: false,
			button: 1,
			relatedTarget: undefined
		};
		evt.initMouseEvent(type, 
		e.bubbles, e.cancelable, e.view, e.detail,
		e.screenX, e.screenY, e.clientX, e.clientY,
		e.ctrlKey, e.altKey, e.shiftKey, e.metaKey,
		e.button, node);
		setTimeout(function() {
			node.dispatchEvent(evt);
			cb(cb_param);
		}, delay);
	};

	function doTest(step_id) {
		var node = document.querySelector('div#joystick div.back');
		switch(step_id) {
			case 0:
				break;
			case 1:
				if ((target_x == target_x_cur) && (target_y == target_y_cur)) {
					step_id += 1;
				}
				target_x_cur += Math.sign(target_x - target_x_cur);
				target_y_cur += Math.sign(target_y - target_y_cur);
				mouseEvent(node, 'mousemove', target_x_cur, target_y_cur, doTest, step_id, 10);
				break;
			default:
				mouseEvent(node, 'mouseup', target_x_cur, target_y_cur, startTest, 0, 200);
				break;
		}
	};

	function startTest(step_id) {
		target_x = Math.floor((Math.random() * 300) - 150);
		target_y = Math.floor((Math.random() * 300) - 150);
		target_x_cur = 0;
		target_y_cur = 0;
		var node = document.querySelector('div#joystick div.back');
		mouseEvent(node, 'mousedown', target_x_cur, target_y_cur, doTest, step_id + 1, 100);
	};

	startTest(0);
}

