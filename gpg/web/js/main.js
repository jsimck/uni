var sDrawCar = false;
var carI = 0;

var uiHandler = new UIHandler();

function setup() {
	var canvas = createCanvas(800,600);
	canvas.parent('canvas');
	background(48, 48, 48);
	stroke(255);
	fill(196);
}

function draw() {
	scale(1, -1);
	translate(0, -height);
	background(48, 48, 48);
	smooth();

	// Draw all uiHandler.curves already drawen
	for (var i = 0; i < uiHandler.curves.length; i++) {
		uiHandler.curves[i].draw();
	}

	// Draw currently in progress curve
	if (uiHandler.tmpCurve != null) {
		uiHandler.tmpCurve.draw();
	}
}

function mouseMoved() {
	switch (uiHandler.tool) {
		case "ferguson":
			switch (uiHandler.state) {
				case 1:
					uiHandler.tmpCurve.P1.x = mouseX;
					uiHandler.tmpCurve.P1.y = height - mouseY;
					uiHandler.tmpCurve.P1_.x = mouseX;
					uiHandler.tmpCurve.P1_.y = height - mouseY;
					break;

				case 2:
					uiHandler.tmpCurve.P0_.x = mouseX;
					uiHandler.tmpCurve.P0_.y = height - mouseY;
					break;

				case 3:
					uiHandler.tmpCurve.P1_.x = mouseX;
					uiHandler.tmpCurve.P1_.y = height - mouseY;
					break;
			}
			break;

		case "bezier":
			switch (uiHandler.state) {
				case 1:
					uiHandler.tmpCurve.P1.x = mouseX;
					uiHandler.tmpCurve.P1.y = height - mouseY;
					uiHandler.tmpCurve.P2.x = mouseX;
					uiHandler.tmpCurve.P2.y = height - mouseY;
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					break;

				case 2:
					uiHandler.tmpCurve.P2.x = mouseX;
					uiHandler.tmpCurve.P2.y = height - mouseY;
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					break;

				case 3:
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					break;
			}
			break;

		default:
			break;
	}
}

function mousePressed() {
	switch (uiHandler.tool) {
		case "ferguson":
			switch (uiHandler.state) {
				case 0:
					uiHandler.tmpCurve = new CurveFerguson();
					uiHandler.tmpCurve.setAll(mouseX, height - mouseY);
					uiHandler.nextState();
					break;

				case 1:
					uiHandler.tmpCurve.P1.x = mouseX;
					uiHandler.tmpCurve.P1.y = height - mouseY;
					uiHandler.tmpCurve.P1_.x = mouseX;
					uiHandler.tmpCurve.P1_.y = height - mouseY;
					uiHandler.nextState();
					break;

				case 2:
					uiHandler.tmpCurve.P0_.x = mouseX;
					uiHandler.tmpCurve.P0_.y = height - mouseY;
					uiHandler.nextState();
					break;

				case 3:
					uiHandler.tmpCurve.P1_.x = mouseX;
					uiHandler.tmpCurve.P1_.y = height - mouseY;
					uiHandler.curves.push(uiHandler.tmpCurve);
					uiHandler.resetState();
					break;
			}
			break;

		case "bezier":
			switch (uiHandler.state) {
				case 0:
					uiHandler.tmpCurve = new CurveBezier();
					uiHandler.tmpCurve.setAll(mouseX, height - mouseY);
					uiHandler.nextState();
					break;

				case 1:
					uiHandler.tmpCurve.P1.x = mouseX;
					uiHandler.tmpCurve.P1.y = height - mouseY;
					uiHandler.tmpCurve.P2.x = mouseX;
					uiHandler.tmpCurve.P2.y = height - mouseY;
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					uiHandler.nextState();
					break;

				case 2:
					uiHandler.tmpCurve.P2.x = mouseX;
					uiHandler.tmpCurve.P2.y = height - mouseY;
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					uiHandler.nextState();
					break;

				case 3:
					uiHandler.tmpCurve.P3.x = mouseX;
					uiHandler.tmpCurve.P3.y = height - mouseY;
					uiHandler.curves.push(uiHandler.tmpCurve);
					uiHandler.resetState();
					break;
			}
			break;

		default:
			break;
	}
}

function drawCar(curve) {
	stroke(255);
	for (var i = 0.01; i < 1; i += 0.01) {
		var carP = curve.P(carI);
		var carPd = curve.Pd(carI);
		line(carP.x, carP.y, carPd.x, carPd.y);
		drawArrow(carP.x, carP.y, 50);
		carI += 0.00005;

		if (carI > 1) {
			carI = 0;
			sDrawCar = false;
		}
	}
}

function reset() {
	uiHandler.curves = [];
	uiHandler.tmpCurve = null;
	uiHandler.resetState();
	sDrawCar = false;
	carI = 0;
}

function drawArrow(x, y, angle) {
	push();

	translate(x,y);
	rotate(angle);

	fill(155,89,182);
	stroke(155,89,182);

	strokeWeight(3);
	line(0, 0, 20, 0);

	strokeWeight(1);
  triangle(20, 6, 20, -6, 30, 0);

	pop();
}

function rocknroll() {
	sDrawCar = true;
	uiHandler.resetState();
}
