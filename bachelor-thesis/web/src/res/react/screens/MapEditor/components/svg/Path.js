import React, { Component } from 'react'
import _ from 'lodash'
import PathHelpers from '../../helpers/PathHelpers'
import GridHelpers from '../../helpers/GridHelpers'
import classNames from 'classnames'

class Path extends Component {
    constructor(props) {
        super(props);

        // Set default state
        this.state = {
            dragging: false,
            handle: '',
            point: [],
            pointIndex: -1,
            originPointX: 0,
            originPointY: 0,
            originD: [],
            originX: 0,
            originY: 0
        };
    }

    handleMouseDownEvent(event, handle, pointIndex, point) {
        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                point: point || [],
                pointIndex: (!_.isUndefined(pointIndex) && pointIndex >= 0) ? pointIndex : -1,
                originPointX: !_.isUndefined(point) ? point[0] : 0,
                originPointY: !_.isUndefined(point) ? point[1] : 0,
                originD: this.props.item.attr.d,
                originX: event.nativeEvent.offsetX,
                originY: event.nativeEvent.offsetY
            });
        }
    }

    handleMouseUpEvent() {
        this.setState({
            handle: '',
            dragging: false,
            point: [],
            pointIndex: -1,
            originPointX: 0,
            originPointY: 0,
            originD: [],
            originX: 0,
            originY: 0
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
                    var point = [];
                    var originPoint = [];
                    var pointsLength = nextProps.item.attr.d.length;

                    // Set new points array
                    nextProps.item.attr.d = _.map(nextProps.item.attr.d, (d, i) => {
                        // Parse points to arrays
                        point = PathHelpers.parseStringPoint(d);
                        originPoint = PathHelpers.parseStringPoint(this.state.originD[i]);

                        // Assign new moved coordinate
                        point[0] = originPoint[0] + this.state.originPointX - (this.state.originX - offsetX);
                        point[1] = originPoint[1] + this.state.originPointY - (this.state.originY - offsetY);

                        // Check if grid is active and apply computation of new X and Y
                        if (this.props.grid.enabled) {
                            point[0] = GridHelpers.computeGrid(point[0], this.props.grid.size);
                            point[1] = GridHelpers.computeGrid(point[1], this.props.grid.size);
                        }

                        // Make string again from array
                        point = point.join(' ');

                        if (i == pointsLength - 1) {
                            // Last item, add Z
                            point += 'Z';
                        } else if (i == 0) {
                            // First item, add M
                            point = 'M' + point;
                        } else {
                            // Other points add L
                            point = 'L' + point;
                        }

                        // Push to new array
                        return point;
                    });
                    break;

                case 'point':
                    this.state.point[0] = this.state.originPointX - (this.state.originX - offsetX);
                    this.state.point[1] = this.state.originPointY - (this.state.originY - offsetY);

                    // Check if grid is active and apply computation of new X and Y
                    if (this.props.grid.enabled) {
                        this.state.point[0] = GridHelpers.computeGrid(this.state.point[0], this.props.grid.size);
                        this.state.point[1] = GridHelpers.computeGrid(this.state.point[1], this.props.grid.size);
                    }

                    // Merge points to string
                    nextProps.item.attr.d[this.state.pointIndex] = this.state.point.join(' ');

                    // Add correct path characters
                    if (this.state.pointIndex == nextProps.item.attr.d.length - 1) {
                        // Last item, add Z
                        nextProps.item.attr.d[this.state.pointIndex] += 'Z';
                    } else if (this.state.pointIndex == 0) {
                        // First item, add M
                        nextProps.item.attr.d[this.state.pointIndex] = 'M' + nextProps.item.attr.d[this.state.pointIndex];
                    } else {
                        // Other points add L
                        nextProps.item.attr.d[this.state.pointIndex] = 'L' + nextProps.item.attr.d[this.state.pointIndex];
                    }
                    break;
            }
        }
    }

    render() {
        // Get first point for action icon position
        var firstPoint = PathHelpers.parseStringPoint(this.props.item.attr.d[0]);

        return (
            <g onMouseUp={() => this.handleMouseUpEvent()}>
                <path
                    className={classNames({'cursor-move': this.props.item.data.selected})}
                    onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                    d={this.props.item.attr.d.join(' ')}
                    data-index={this.props.index}
                    style={{...this.props.item.style}}
                />

                {(this.props.item.action.hasOwnProperty('title')) &&
                    <g className="action-icon">
                        <text x={firstPoint[0]} y={firstPoint[1]}>&#xf0e7;</text>
                    </g>
                }

                {this.props.item.data.selected &&
                    <g className="resizer">
                        {_.map(PathHelpers.getPointsArray(this.props.item.attr.d), (entry, index) => {
                            return <circle
                                onMouseDown={(event) => this.handleMouseDownEvent(event, 'point', index, [entry[0], entry[1]])}
                                className="point-move"
                                cx={entry[0]}
                                cy={entry[1]}
                                key={index}
                                r="5"
                            />
                        })}
                    </g>
                }
            </g>
        );
    }
}

Path.propTypes = {
    index: React.PropTypes.number,
    grid: React.PropTypes.object,
    item: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Path