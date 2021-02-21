import React, { Component } from 'react'
import GridHelpers from '../../helpers/GridHelpers'

class Circle extends Component {
    constructor(props) {
        super(props);

        // Set default state
        this.state = {
            dragging: false,
            handle: '',
            originX: 0,
            originCursorX: 0,
            originCursorY: 0,
            originRadius: 0
        };
    }

    handleMouseDownEvent(event, handle) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originX: event.nativeEvent.offsetX,
                originCursorX: event.nativeEvent.offsetX - this.props.item.attr.cx,
                originCursorY: event.nativeEvent.offsetY - this.props.item.attr.cy,
                originRadius: this.props.item.attr.r
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            dragging: false,
            handle: '',
            originX: 0,
            originCursorX: 0,
            originCursorY: 0,
            originRadius: 0
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
                    nextProps.item.attr.r = this.state.originRadius + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.r = GridHelpers.computeGrid(nextProps.item.attr.r, this.props.grid.size);
                    }
                    break;

                case 'top-right':
                    nextProps.item.attr.r = this.state.originRadius - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.r = GridHelpers.computeGrid(nextProps.item.attr.r, this.props.grid.size);
                    }
                    break;

                case 'bottom-right':
                    nextProps.item.attr.r = this.state.originRadius - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.r = GridHelpers.computeGrid(nextProps.item.attr.r, this.props.grid.size);
                    }
                    break;

                case 'bottom-left':
                    nextProps.item.attr.r = this.state.originRadius + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.r = GridHelpers.computeGrid(nextProps.item.attr.r, this.props.grid.size);
                    }
                    break;
            }
        }
    }

    render() {
        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <circle
                    {...this.props.item.attr}
                    data-index={this.props.index}
                    style={{...this.props.item.style}}
                />

                {(this.props.item.action.hasOwnProperty('title')) &&
                    <g className="action-icon">
                        <text
                            x={(this.props.item.attr.cx + this.props.item.attr.r) - 15}
                            y={(this.props.item.attr.cy - this.props.item.attr.r) + 20}
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
                            x={this.props.item.attr.cx - this.props.item.attr.r}
                            y={this.props.item.attr.cy - this.props.item.attr.r}
                            width={this.props.item.attr.r * 2}
                            height={this.props.item.attr.r * 2}
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-left')}
                            className="point-top-left"
                            cx={this.props.item.attr.cx - this.props.item.attr.r}
                            cy={this.props.item.attr.cy - this.props.item.attr.r}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-right')}
                            className="point-top-right"
                            cx={this.props.item.attr.cx + this.props.item.attr.r}
                            cy={this.props.item.attr.cy - this.props.item.attr.r}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-left')}
                            className="point-bottom-left"
                            cx={this.props.item.attr.cx - this.props.item.attr.r}
                            cy={this.props.item.attr.cy + this.props.item.attr.r}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-right')}
                            className="point-bottom-right"
                            cx={this.props.item.attr.cx + this.props.item.attr.r}
                            cy={this.props.item.attr.cy + this.props.item.attr.r}
                            r="5"
                        />
                    </g>
                }
            </g>
        );
    }
}

Circle.propTypes = {
    index: React.PropTypes.number,
    grid: React.PropTypes.object,
    item: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Circle