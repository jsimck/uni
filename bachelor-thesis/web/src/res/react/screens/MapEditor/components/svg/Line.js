import React, { Component } from 'react'
import GridHelpers from '../../helpers/GridHelpers'
import classNames from 'classnames'

class Line extends Component {
    constructor(props) {
        super(props);

        // Set default state
        this.state = {
            dragging: false,
            handle: '',
            originX: 0,
            originY: 0,
            originStartX: 0,
            originStartY: 0,
            originEndX: 0,
            originEndY: 0
        };
    }

    handleMouseDownEvent(event, handle) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originX: event.nativeEvent.offsetX,
                originY: event.nativeEvent.offsetY,
                originStartX: this.props.item.attr.x1,
                originStartY: this.props.item.attr.y1,
                originEndX: this.props.item.attr.x2,
                originEndY: this.props.item.attr.y2
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            dragging: false,
            handle: '',
            originX: 0,
            originY: 0,
            originStartX: 0,
            originStartY: 0,
            originEndX: 0,
            originEndY: 0
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
                    nextProps.item.attr.x1 = this.state.originStartX - (this.state.originX - offsetX);
                    nextProps.item.attr.y1 = this.state.originStartY - (this.state.originY - offsetY);
                    nextProps.item.attr.x2 = this.state.originEndX - (this.state.originX - offsetX);
                    nextProps.item.attr.y2 = this.state.originEndY - (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x1 = GridHelpers.computeGrid(nextProps.item.attr.x1, this.props.grid.size);
                        nextProps.item.attr.y1 = GridHelpers.computeGrid(nextProps.item.attr.y1, this.props.grid.size);
                        nextProps.item.attr.x2 = GridHelpers.computeGrid(nextProps.item.attr.x2, this.props.grid.size);
                        nextProps.item.attr.y2 = GridHelpers.computeGrid(nextProps.item.attr.y2, this.props.grid.size);
                    }
                    break;

                case 'start':
                    nextProps.item.attr.x1 = offsetX;
                    nextProps.item.attr.y1 = offsetY;

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x1 = GridHelpers.computeGrid(nextProps.item.attr.x1, this.props.grid.size);
                        nextProps.item.attr.y1 = GridHelpers.computeGrid(nextProps.item.attr.y1, this.props.grid.size);
                    }
                    break;

                case 'end':
                    nextProps.item.attr.x2 = offsetX;
                    nextProps.item.attr.y2 = offsetY;

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        nextProps.item.attr.x2 = GridHelpers.computeGrid(nextProps.item.attr.x2, this.props.grid.size);
                        nextProps.item.attr.y2 = GridHelpers.computeGrid(nextProps.item.attr.y2, this.props.grid.size);
                    }
                    break;
            }
        }
    }

    render() {
        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <line
                    onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                    style={{...this.props.item.style}}
                    className={classNames({'cursor-move': this.props.item.data.selected})}
                    data-index={this.props.index}
                    {...this.props.item.attr}
                />

                {this.props.item.action.hasOwnProperty('title') &&
                    <g className="action-icon">
                        <text
                            x={(this.props.item.attr.x2) - 15}
                            y={(this.props.item.attr.y2) - 10}
                        >
                            &#xf0e7;
                        </text>
                    </g>
                }

                {this.props.item.data.selected &&
                    <g className="resizer">
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'start')}
                            className="point-move"
                            cx={this.props.item.attr.x1}
                            cy={this.props.item.attr.y1}
                            r="5"
                        />
                        <circle
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'end')}
                            className="point-move"
                            cx={this.props.item.attr.x2}
                            cy={this.props.item.attr.y2}
                            r="5"
                        />
                    </g>
                }
            </g>
        );
    }
}

Line.propTypes = {
    index: React.PropTypes.number,
    grid: React.PropTypes.object,
    item: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Line