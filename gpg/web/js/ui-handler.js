function UIHandler() {
  this.tool = "ferguson";
  this.state = 0;
  this.curves = [];
  this.tmpCurve = null;
}

UIHandler.prototype.setTool = function(tool) {
  this.tool = tool;
  this.resetState();
}

UIHandler.prototype.resetState = function() {
  this.state = 0;
  this.tmpCurve = null;
}

UIHandler.prototype.nextState = function() {
  this.state++;
}

UIHandler.prototype.clear = function() {
  this.state = 0;
  this.convert = false;
  this.curves = [];
  this.tmpCurve = null;
}

UIHandler.prototype.convert = function() {
  this.resetState();
  this.setTool("");

  for (var i = 0; i < this.curves.length; i++) {
    if (this.curves[i] instanceof CurveFerguson && this.curves[i].bezier == false) {
      this.curves[i].bezier = true;
      break;
    }
  }
}
