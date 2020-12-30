function CurveFerguson() {
	this.tMatrix = [
  	[2, -2, 1, 1],
  	[-3, 3, -2, -1],
  	[0, 0, 1, 0],
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
	this.P0_ = {
		x: 2,
		y: 1
	};
	this.P1_ = {
		x: 1,
		y: 1
	};

	this.bezier = false;
}

CurveFerguson.prototype.V0 = function() {
  return {
    x: this.P0_.x - this.P0.x,
    y: this.P0_.y - this.P0.y
  };
}

CurveFerguson.prototype.V1 = function() {
  return {
    x: this.P1_.x - this.P1.x,
    y: this.P1_.y - this.P1.y
  };
}

CurveFerguson.prototype.V0 = function() {
  return {
    x: this.P0_.x - this.P0.x,
    y: this.P0_.y - this.P0.y
  };
}

CurveFerguson.prototype.V1 = function() {
  return {
    x: this.P1_.x - this.P1.x,
    y: this.P1_.y - this.P1.y
  };
}

CurveFerguson.prototype.T = function(t) {
	return [t * t * t, t * t, t, 1];
}

CurveFerguson.prototype.Td = function(t) {
	return [3 * t * t, 2 * t, 1, 0];
}

CurveFerguson.prototype.P = function(t) {
	return {
    x: math.multiply(
      math.multiply(this.T(t), this.tMatrix),
      [this.P0.x, this.P1.x, this.V0().x, this.V1().x]
    ),
    y: math.multiply(
      math.multiply(this.T(t), this.tMatrix),
      [this.P0.y, this.P1.y, this.V0().y, this.V1().y]
    )
  };
}

CurveFerguson.prototype.Pd = function(t) {
	return {
    x: math.multiply(
      math.multiply(this.Td(t), this.tMatrix),
      [this.P0.x, this.P1.x, this.V0().x, this.V1().x]
    ),
    y: math.multiply(
      math.multiply(this.Td(t), this.tMatrix),
      [this.P0.y, this.P1.y, this.V0().y, this.V1().y]
    )
  };
}

CurveFerguson.prototype.setAll = function(x, y) {
  this.P0.x = x;
  this.P0.y = y;
	this.P1.x = x;
	this.P1.y = y;
	this.P0_.x = x;
	this.P0_.y = y;
	this.P1_.x = x;
	this.P1_.y = y;
}

CurveFerguson.prototype.draw = function() {
	if (this.bezier) {
		this.drawBezier();
	} else {
		this.drawNormal();
	}
}

CurveFerguson.prototype.drawNormal = function() {
	push();

	// Curve
	strokeWeight(1);
	stroke(255);
	var P1 = this.P(0);
	for (var i = 0.01; i < 1; i += 0.01) {
		var P2 = this.P(i);
		line(P1.x, P1.y, P2.x, P2.y);
		P1 = P2;
	}

	// Vectors
	stroke(46,204,113);
	line(this.P0_.x, this.P0_.y, this.P0.x,this.P0.y);
	line(this.P1_.x, this.P1_.y, this.P1.x,this.P1.y);

	// Points
	strokeWeight(10);
	stroke(192,57,43);
	point(this.P0.x,this.P0.y);
	point(this.P1.x,this.P1.y);

	stroke(22,160,133);
	point(this.P0_.x, this.P0_.y);
	point(this.P1_.x, this.P1_.y);

	pop();
}

CurveFerguson.prototype.drawBezier = function() {
	push();

	// Curve
	strokeWeight(1);
	stroke(255);
	var P1 = this.P(0);
	for (var i = 0.01; i < 1; i += 0.01) {
		var P2 = this.P(i);
		line(P1.x, P1.y, P2.x, P2.y);
		P1 = P2;
	}

	// Vectors
	stroke(46,204,113);
	line(this.P0.x, this.P0.y, this.P0.x + (1/3 * this.V0().x), this.P0.y + (1/3 * this.V0().y));
	line(this.P0.x + (1/3 * this.V0().x), this.P0.y + (1/3 * this.V0().y), this.P1.x - (1/3 * this.V1().x), this.P1.y - (1/3 * this.V1().y));
	line(this.P1.x - (1/3 * this.V1().x), this.P1.y - (1/3 * this.V1().y), this.P1.x, this.P1.y);

	// Points
	strokeWeight(10);
	stroke(192,57,43);
	point(this.P0.x,this.P0.y);
	point(this.P1.x,this.P1.y);

	stroke(22,160,133);
	point(this.P0.x + (1/3 * this.V0().x), this.P0.y + (1/3 * this.V0().y));
	point(this.P1.x - (1/3 * this.V1().x), this.P1.y - (1/3 * this.V1().y));

	pop();
}
