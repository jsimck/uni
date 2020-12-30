function CurveBezier() {
	this.tMatrix = [
  	[-1, 3, -3, 1],
  	[3, -6, 3, 0],
  	[-3, 3, 0, 0],
  	[1, 0, 0, 0]
  ];

	this.P0 = {
		x: 1,
		y: 0
	};
	this.P1 = {
		x: 2,
		y: 2
	};
	this.P2 = {
		x: 2,
		y: 1
	};
	this.P3 = {
		x: 1,
		y: 1
	};
}

CurveBezier.prototype.T = function(t) {
	return [t * t * t, t * t, t, 1];
}

CurveBezier.prototype.P = function(t) {
	return {
    x: math.multiply(
      math.multiply(this.T(t), this.tMatrix),
      [this.P0.x, this.P1.x, this.P2.x, this.P3.x]
    ),
    y: math.multiply(
      math.multiply(this.T(t), this.tMatrix),
      [this.P0.y, this.P1.y, this.P2.y, this.P3.y]
    )
  };
}

CurveBezier.prototype.setAll = function(x, y) {
  this.P0.x = x;
  this.P0.y = y;
	this.P1.x = x;
	this.P1.y = y;
	this.P2.x = x;
	this.P2.y = y;
	this.P3.x = x;
	this.P3.y = y;
}

CurveBezier.prototype.draw = function() {
	push();

	// Curve
	strokeWeight(1);
	stroke(3,169,244);
	var P1 = this.P(0);
	for (var i = 0.01; i < 1; i += 0.01) {
		var P2 = this.P(i);
		line(P1.x, P1.y, P2.x, P2.y);
		P1 = P2;
	}

	// Vectors
	stroke(126,87,194);
	line(this.P0.x, this.P0.y, this.P1.x,this.P1.y);
	line(this.P1.x, this.P1.y, this.P2.x,this.P2.y);
	line(this.P2.x, this.P2.y, this.P3.x,this.P3.y);

	// Points
	strokeWeight(10);
	stroke(0,131,143);
	point(this.P0.x,this.P0.y);
	point(this.P3.x, this.P3.y);

	stroke(173,20,87);
	point(this.P2.x, this.P2.y);
	point(this.P1.x,this.P1.y);

	pop();
}
