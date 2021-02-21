import React, { Component } from 'react'
import GridHelpers from '../../helpers/GridHelpers'

class Ellipse extends Component {
    constructor(props) {
        super(props);

        // Set default state
        this.state = {
            dragging: false,
            handle: '',
            originX: 0,
            originY: 0,
            originCursorX: 0,
            originCursorY: 0,
            originRadiusX: 0,
            originRadiusY: 0
        };
    }

    handleMouseDownEvent(event, handle) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originX: event.nativeEvent.offsetX,
                originY: event.nativeEvent.offsetY,
                originCursorX: event.nativeEvent.offsetX - this.props.item.attr.cx,
                originCursorY: event.nativeEvent.offsetY - this.props.item.attr.cy,
                originRadiusX: this.props.item.attr.rx,
                originRadiusY: this.props.item.attr.ry
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            dragging: false,
            handle: '',
            originX: 0,
            originY: 0,
            originCursorX: 0,
            originCursorY: 0,
            originRadiusX: 0,
            originRadiusY: 0
        });

        // Update svg
        this.props.onUpdateSvg(this.props.index, this.props.item);
    }

    componentWillUpdate(nextProps) {
        if (nextProps.item.data.selected && this.state.dragging) {
            // Cache mouse position
            var offsetX = nextProps.eventMouseMove.nativeEvent.offsetX;
            var offsetY = nextProps.eventMouseMove.nativeEvent.offsetY;

            switch (this.state.handle) {
                case 'move':
                    nextProps.item.attr.cx = (offsetX - this.state.originCursorX);
                    nextProps.item.attr.cy = (offsetY - this.state.originCursorY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.cx = GridHelpers.computeGrid(nextProps.item.attr.cx, this.props.grid.size);
                        nextProps.item.attr.cy = GridHelpers.computeGrid(nextProps.item.attr.cy, this.props.grid.size);
                    }
                    break;

                case 'top-left':
                    nextProps.item.attr.ry = this.state.originRadiusY + (this.state.originY - offsetY);
                    nextProps.item.attr.rx = this.state.originRadiusX + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;

                case 'top-right':
                    nextProps.item.attr.ry = this.state.originRadiusY + (this.state.originY - offsetY);
                    nextProps.item.attr.rx = this.state.originRadiusX - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;

                case 'bottom-left':
                    nextProps.item.attr.ry = this.state.originRadiusY - (this.state.originY - offsetY);
                    nextProps.item.attr.rx = this.state.originRadiusX + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;

                case 'bottom-right':
                    nextProps.item.attr.ry = this.state.originRadiusY - (this.state.originY - offsetY);
                    nextProps.item.attr.rx = this.state.originRadiusX - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;

                case 'top':
                    nextProps.item.attr.ry = this.state.originRadiusY + (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                    }
                    break;

                case 'bottom':
                    nextProps.item.attr.ry = this.state.originRadiusY - (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.ry = GridHelpers.computeGrid(nextProps.item.attr.ry, this.props.grid.size);
                    }
                    break;

                case 'right':
                    nextProps.item.attr.rx = this.state.originRadiusX - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;

                case 'left':
                    nextProps.item.attr.rx = this.state.originRadiusX + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.rx = GridHelpers.computeGrid(nextProps.item.attr.rx, this.props.grid.size);
                    }
                    break;
            }
        }
    }

    render() {
        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <ellipse
                    {...this.props.item.attr}
                    data-index={this.props.index}
                    style={{...this.props.item.style}}
                />

                {(this.props.item.action.hasOwnProperty('title')) &&
                    <g className="action-icon">
                        <text
                            x={(this.props.item.attr.cx + this.props.item.attr.rx) - 15}
                            y={(this.props.item.attr.cy - this.props.item.attr.ry) + 20}
                        >
                            &#xf0e7;
                        </text>
                    </g>
                }

                {this.props.item.data.selected &&
                    <g className="resizer">
                        <rect
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                            data-index={this.props.index}
                            className="resizer-rect"
                            fill="none"
                            x={this.props.item.attr.cx - this.props.item.attr.rx}
                            y={this.props.item.attr.cy - this.props.item.attr.ry}
                            width={this.props.item.attr.rx* 2}
                            height={this.props.item.attr.ry * 2}
                        />

                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-left')}
                            className="point-top-left"
                            cx={this.props.item.attr.cx - this.props.item.attr.rx}
                            cy={this.props.item.attr.cy - this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top')}
                            className="point-top"
                            cx={this.props.item.attr.cx}
                            cy={this.props.item.attr.cy - this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-right')}
                            className="point-top-right"
                            cx={this.props.item.attr.cx + this.props.item.attr.rx}
                            cy={this.props.item.attr.cy - this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'right')}
                            className="point-right"
                            cx={this.props.item.attr.cx + this.props.item.attr.rx}
                            cy={this.props.item.attr.cy}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-left')}
                            className="point-bottom-left"
                            cx={this.props.item.attr.cx - this.props.item.attr.rx}
                            cy={this.props.item.attr.cy + this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom')}
                            className="point-bottom"
                            cx={this.props.item.attr.cx}
                            cy={this.props.item.attr.cy + this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-right')}
                            className="point-bottom-right"
                            cx={this.props.item.attr.cx + this.props.item.attr.rx}
                            cy={this.props.item.attr.cy + this.props.item.attr.ry}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'left')}
                            className="point-left"
                            cx={this.props.item.attr.cx - this.props.item.attr.rx}
                            cy={this.props.item.attr.cy}
                            r="5"
                        />
                    </g>
                }
            </g>
        );
    }
}

Ellipse.propTypes = {
    index: React.PropTypes.number,
    grid: React.PropTypes.object,
    item: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Ellipse