import React, { Component } from 'react'
import _ from 'lodash'
import GridHelpers from '../../helpers/GridHelpers'

class Text extends Component {
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

    updateTextBBox(target) {
        var bbox = target.getBBox();
        this.props.item.data.height = bbox.height;
        this.props.item.data.width = bbox.width;
    }

    handleMouseDownEvent(event, handle) {
        // Update text width and height
        this.updateTextBBox(event.target);
        this.props.onUpdateSvg(this.props.index, this.props.item);

        if (this.props.item.data.selected) {
            this.setState({
                handle: handle,
                dragging: true,
                originCursorX: event.nativeEvent.offsetX - this.props.item.attr.x,
                originCursorY: event.nativeEvent.offsetY - this.props.item.attr.y
            });
        }
    }

    handleMouseUpEvent(event) {
        // Update text width and height
        this.updateTextBBox(event.target);

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
            switch (this.state.handle) {
                case 'move':
                    nextProps.item.attr.x = (nextProps.eventMouseMove.nativeEvent.offsetX - this.state.originCursorX);
                    nextProps.item.attr.y = (nextProps.eventMouseMove.nativeEvent.offsetY - this.state.originCursorY);

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
            <g onMouseUp={(event) => this.handleMouseUpEvent(event)}>
                <text
                    onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                    data-index={this.props.index}
                    className="svg-text"
                    style={{...this.props.item.style}}
                    {...this.props.item.attr}
                >
                    {_.map(this.props.item.data.content.split('\n'), (entry, index) => {
                        return <tspan key={index} x={this.props.item.attr.x} dy="1.2em">{entry}</tspan>;
                    })}
                </text>

                {(this.props.item.action.hasOwnProperty('title')) &&
                    <g className="action-icon">
                        <text x={(this.props.item.attr.x + 5)} y={(this.props.item.attr.y) + 20}>&#xf0e7;</text>
                    </g>
                }

                {this.props.item.data.selected &&
                    <g className="resizer">
                        <rect
                            onMouseDown={(event) => this.handleMouseDownEvent(event, 'move')}
                            data-index={this.props.index}
                            className="resizer-rect"
                            fill="none"
                            width={this.props.item.data.width}
                            height={this.props.item.data.height}
                            x={this.props.item.attr.x}
                            y={this.props.item.attr.y}
                        />
                    </g>
                }
            </g>
        );
    }
}

Text.propTypes = {
    index: React.PropTypes.number,
    item: React.PropTypes.object,
    grid: React.PropTypes.object,
    onUpdateSvg: React.PropTypes.func,
    eventMouseMove: React.PropTypes.object
};

export default Text
