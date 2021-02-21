import React, { Component } from 'react'
import GridHelpers from '../../helpers/GridHelpers'

class Circle extends Component {
    constructor(props) {
        super(props);

        // Set default state
        this.state = {
            dragging: false,
            handle: '',
            originCursorX: 0,
            originCursorY: 0
        };
    }

    handleMouseDownEvent(event, handle) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originCursorX: event.nativeEvent.offsetX - this.props.item.attr.x,
                originCursorY: event.nativeEvent.offsetY - this.props.item.attr.y
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            dragging: false,
            handle: '',
            originCursorX: 0,
            originCursorY: 0
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
            }
        }
    }

    render() {
        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <g
                    data-index={this.props.index}
                    className="action-icon"
                    style={{fontSize: 30}}
                >
                    <text
                        className="position"
                        data-index={this.props.index}
                        fill="white"
                        x={this.props.item.attr.x - 12}
                        y={this.props.item.attr.y + 11}
                    >
                        &#xf111;
                    </text>
                    <text
                        className="position"
                        data-index={this.props.index}
                        fill="#f55753"
                        x={this.props.item.attr.x - 12}
                        y={this.props.item.attr.y + 11}
                    >
                        &#xf192;
                    </text>
                </g>

                {this.props.item.data.selected &&
                    <g className="resizer">
                        <rect
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                            data-index={this.props.index}
                            className="resizer-rect"
                            fill="none"
                            x={this.props.item.attr.x - 13}
                            y={this.props.item.attr.y - 13}
                            width={27}
                            height={27}/>
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
