import React, { Component } from 'react'
import GridHelpers from '../../helpers/GridHelpers'

class Rect extends Component {
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
            originWidth: 0,
            originHeight: 0
        };
    }

    handleMouseDownEvent(event, handle) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originX: event.nativeEvent.offsetX,
                originY: event.nativeEvent.offsetY,
                originCursorX: event.nativeEvent.offsetX - this.props.item.attr.x,
                originCursorY: event.nativeEvent.offsetY - this.props.item.attr.y,
                originWidth: this.props.item.attr.width,
                originHeight: this.props.item.attr.height
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            dragging: false,
            handle: ''
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
                    nextProps.item.attr.x = (offsetX - this.state.originCursorX);
                    nextProps.item.attr.y = (offsetY - this.state.originCursorY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x = GridHelpers.computeGrid(nextProps.item.attr.x, this.props.grid.size);
                        nextProps.item.attr.y = GridHelpers.computeGrid(nextProps.item.attr.y, this.props.grid.size);
                    }
                    break;

                case 'top-left':
                    nextProps.item.attr.x = this.state.originX - (this.state.originX - offsetX);
                    nextProps.item.attr.y = this.state.originY - (this.state.originY - offsetY);
                    nextProps.item.attr.width = this.state.originWidth + (this.state.originX - offsetX);
                    nextProps.item.attr.height = this.state.originHeight + (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x = GridHelpers.computeGrid(nextProps.item.attr.x, this.props.grid.size);
                        nextProps.item.attr.y = GridHelpers.computeGrid(nextProps.item.attr.y, this.props.grid.size);
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'top':
                    nextProps.item.attr.y = this.state.originY - (this.state.originY - offsetY);
                    nextProps.item.attr.height = this.state.originHeight + (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.y = GridHelpers.computeGrid(nextProps.item.attr.y, this.props.grid.size);
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'top-right':
                    nextProps.item.attr.y = this.state.originY - (this.state.originY - offsetY);
                    nextProps.item.attr.width = this.state.originWidth - (this.state.originX - offsetX);
                    nextProps.item.attr.height = this.state.originHeight + (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.y = GridHelpers.computeGrid(nextProps.item.attr.y, this.props.grid.size);
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'right':
                    nextProps.item.attr.width = this.state.originWidth - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                    }
                    break;

                case 'bottom-right':
                    nextProps.item.attr.height = this.state.originHeight - (this.state.originY - offsetY);
                    nextProps.item.attr.width = this.state.originWidth - (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'bottom':
                    nextProps.item.attr.height = this.state.originHeight - (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'bottom-left':
                    nextProps.item.attr.x = this.state.originX - (this.state.originX - offsetX);
                    nextProps.item.attr.width = this.state.originWidth + (this.state.originX - offsetX);
                    nextProps.item.attr.height = this.state.originHeight - (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x = GridHelpers.computeGrid(nextProps.item.attr.x, this.props.grid.size);
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                        nextProps.item.attr.height = GridHelpers.computeGrid(nextProps.item.attr.height, this.props.grid.size);
                    }
                    break;

                case 'left':
                    nextProps.item.attr.x = this.state.originX - (this.state.originX - offsetX);
                    nextProps.item.attr.width = this.state.originWidth + (this.state.originX - offsetX);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x = GridHelpers.computeGrid(nextProps.item.attr.x, this.props.grid.size);
                        nextProps.item.attr.width = GridHelpers.computeGrid(nextProps.item.attr.width, this.props.grid.size);
                    }
                    break;
            }
        }
    }

    render() {
        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <rect
                    {...this.props.item.attr}
                    data-index={this.props.index}
                    style={{...this.props.item.style}}
                />

                {(this.props.item.action.hasOwnProperty('title')) &&
                    <g className="action-icon">
                        <text x={this.props.item.attr.x + this.props.item.attr.width - 15} y={this.props.item.attr.y + 20}>&#xf0e7;</text>
                    </g>
                }

                {this.props.item.data.selected &&
                    <g className="resizer">
                        <rect
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                            data-index={this.props.index}
                            className="resizer-rect"
                            fill="none"
                            width={this.props.item.attr.width}
                            height={this.props.item.attr.height}
                            x={this.props.item.attr.x}
                            y={this.props.item.attr.y}
                        />

                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-left')}
                            className="point-top-left"
                            cx={this.props.item.attr.x}
                            cy={this.props.item.attr.y}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top')}
                            className="point-top"
                            cx={this.props.item.attr.x + (this.props.item.attr.width / 2)}
                            cy={this.props.item.attr.y}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'top-right')}
                            className="point-top-right"
                            cx={this.props.item.attr.x + this.props.item.attr.width}
                            cy={this.props.item.attr.y}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'right')}
                            className="point-right"
                            cx={this.props.item.attr.x + this.props.item.attr.width}
                            cy={this.props.item.attr.y + (this.props.item.attr.height / 2)}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-left')}
                            className="point-bottom-left"
                            cx={this.props.item.attr.x}
                            cy={this.props.item.attr.y + this.props.item.attr.height}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom')}
                            className="point-bottom"
                            cx={this.props.item.attr.x + (this.props.item.attr.width / 2)}
                            cy={this.props.item.attr.y + this.props.item.attr.height}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'bottom-right')}
                            className="point-bottom-right"
                            cx={this.props.item.attr.x + this.props.item.attr.width}
                            cy={this.props.item.attr.y + this.props.item.attr.height}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'left')}
                            className="point-left"
                            cx={this.props.item.attr.x}
                            cy={this.props.item.attr.y + (this.props.item.attr.height / 2)}
                            r="5"
                        />
                    </g>
                }
            </g>
        );
    }
}

Rect.propTypes = {
    index: React.PropTypes.number,
    item: React.PropTypes.object,
    grid: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Rect