import React, { Component } from 'react'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Button } from 'react-bootstrap'
import { canvasSelector } from '../selectors/canvas'
import { editMap } from '../../Maps/actions/maps'
import { addSvg, updateSvg, removeSvg, setSvgSelection, addSvgToSelection, removeSvgFromSelection, clearSvgSelection, clipboardCopy, clipboardCut, clipboardPaste } from '../actions/canvas'
import { setToolState } from '../actions/canvasTool'
import { keyMapSetKey } from '../actions/canvasKeyMap'
import { setMouseMoveEvent } from '../actions/canvasMouseEvent'
import { Rect, Circle, Ellipse, Line, Path, Text, Position } from './svg'
import SVGFactory from '../helpers/SVGFactory'
import PathHelpers from '../helpers/PathHelpers'
import KeyHelpers from '../helpers/KeyHelpers'
import GridHelpers from '../helpers/GridHelpers'

class Canvas extends Component {
    componentDidMount() {
        // Register plain javascript document key events
        document.addEventListener('keydown', (event) => this.handleKeyDown(event), false);
        document.addEventListener('keyup', (event) => this.handleKeyUp(event), false);
    }

    componentWillUnmount() {
        // De-register plain javascript document key events
        document.removeEventListener('keydown', (event) => this.handleKeyDown(event), false);
        document.removeEventListener('keyup', (event) => this.handleKeyUp(event), false);
    }

    handleCanvasDoubleClick(event) {
        // Cache event x and y position
        var offsetX = event.nativeEvent.offsetX;
        var offsetY = event.nativeEvent.offsetY;

        // Check if grid is enabled and compute x and y based on that
        if (this.props.grid.enabled) {
            offsetX = GridHelpers.computeGrid(offsetX, this.props.grid.size);
            offsetY = GridHelpers.computeGrid(offsetY, this.props.grid.size);
        }

        switch (this.props.tool) {
            case 'path':
                switch (this.props.toolState) {
                    case 'drawing':
                        // Create closing point
                        var d = 'L' + offsetX + ' ' + offsetY + ' Z';

                        // Update points array and last index, remove 3 last points inserted due to double && single click event
                        this.props.activeSvg.attr.d[this.props.activeSvg.data.lastIndex - 1] = d;
                        this.props.activeSvg.attr.d = this.props.activeSvg.attr.d.slice(0, this.props.activeSvg.data.lastIndex);
                        this.props.activeSvg.data.lastIndex = this.props.activeSvg.attr.d.length - 1;

                        // Update svg
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        this.props.setToolState('active');
                        break;
                }
                break;
        }
    }

    handleCanvasClick(event) {
        // Cache event x and y position
        var offsetX = event.nativeEvent.offsetX;
        var offsetY = event.nativeEvent.offsetY;

        // Check if grid is enabled and compute x and y based on that
        if (this.props.grid.enabled) {
            offsetX = GridHelpers.computeGrid(offsetX, this.props.grid.size);
            offsetY = GridHelpers.computeGrid(offsetY, this.props.grid.size);
        }

        switch (this.props.tool) {
            case 'move':
                var target = event.target;

                // Check if it's tspan, assign target to text container
                if (target.tagName == 'tspan') {
                    target = target.parentNode;
                }

                if (!_.isNull(target) && target.tagName != 'svg') {
                    // Parse classes to array
                    var targetClasses = !_.isEmpty(target.getAttribute('class')) ? target.getAttribute('class').split(/[\s-]+/) : [];

                    // Skip if target is movable point or resizer
                    if (!_.isEmpty(target.getAttribute('data-index')) && !_.includes(targetClasses, 'point')) {
                        // Get Targets index
                        var index = parseInt(target.getAttribute('data-index'));

                        // Set svg selection
                        if (this.props.keyMap.shift) {
                            if (_.includes(this.props.selectedSvgIndexes, index)) {
                                this.props.removeSvgFromSelection(index);
                            } else {
                                this.props.addSvgToSelection(index);
                            }
                        } else {
                            this.props.setSvgSelection(index);
                        }
                    }
                } else {
                    // Clear all previous selections
                    this.props.clearSvgSelection();
                }
                break;

            case 'path':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Path(offsetX, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;

                    case 'drawing':
                        var d = `L${offsetX} ${offsetY}`;
                        this.props.activeSvg.attr.d.push(d);
                        this.props.activeSvg.data.lastIndex++;
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'position':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Position(offsetX, offsetY));
                        break;
                }
                break;
        }
    }

    handleCanvasMouseDown(event) {
        // Cache event x and y position
        var offsetX = event.nativeEvent.offsetX;
        var offsetY = event.nativeEvent.offsetY;

        // Check if grid is enabled and compute x and y based on that
        if (this.props.grid.enabled) {
            offsetX = GridHelpers.computeGrid(offsetX, this.props.grid.size);
            offsetY = GridHelpers.computeGrid(offsetY, this.props.grid.size);
        }

        switch (this.props.tool) {
            case 'line':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Line(offsetX, offsetX,
                            offsetY, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;
                }
                break;

            case 'rect':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Rect(offsetX, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;
                }
                break;

            case 'ellipse':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Ellipse(offsetX, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;
                }
                break;

            case 'circle':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Circle(offsetX, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;
                }
                break;

            case 'text':
                switch (this.props.toolState) {
                    case 'active':
                        this.props.addSvg(SVGFactory.Text(offsetX, offsetY, this.props.styles));
                        this.props.setToolState('drawing');
                        break;
                }
                break;
        }
    }

    handleCanvasMouseUp() {
        if (_.includes(['line', 'rect', 'ellipse', 'circle', 'text'], this.props.tool) && this.props.toolState == 'drawing') {
            this.props.setToolState('active');
        }
    }

    handleCanvasMouseMove(event) {
        // Cache event x and y position
        var offsetX = event.nativeEvent.offsetX;
        var offsetY = event.nativeEvent.offsetY;

        // Check if grid is enabled and compute x and y based on that
        if (this.props.grid.enabled) {
            offsetX = GridHelpers.computeGrid(offsetX, this.props.grid.size);
            offsetY = GridHelpers.computeGrid(offsetY, this.props.grid.size);
        }

        switch (this.props.tool) {
            case 'line':
                switch (this.props.toolState) {
                    case 'drawing':
                        this.props.activeSvg.attr.x2 = offsetX;
                        this.props.activeSvg.attr.y2 = offsetY;
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'path':
                switch (this.props.toolState) {
                    case 'drawing':
                        // Enable degree snapping if shift key is held
                        var d = PathHelpers.getPathPoint(
                            this.props.keyMap.shift,
                            PathHelpers.parseStringPoint(this.props.activeSvg.attr.d[this.props.activeSvg.data.lastIndex]),
                            [offsetX, offsetY]
                        );

                        this.props.activeSvg.attr.d[this.props.activeSvg.data.lastIndex + 1] = d;
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'rect':
                switch (this.props.toolState) {
                    case 'drawing':
                        this.props.activeSvg.attr.width = offsetX - this.props.activeSvg.attr.x;
                        this.props.activeSvg.attr.height = offsetY - this.props.activeSvg.attr.y;
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'ellipse':
                switch (this.props.toolState) {
                    case 'drawing':
                        this.props.activeSvg.attr.rx = offsetX - this.props.activeSvg.attr.cx;
                        this.props.activeSvg.attr.ry = offsetY - this.props.activeSvg.attr.cy;
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'text':
                switch (this.props.toolState) {
                    case 'drawing':
                        // Test whichever is bigger and apply to circle radius, use 0.5 to reduce threshold
                        if (offsetY - this.props.activeSvg.attr.y > offsetX - this.props.activeSvg.attr.x) {
                            this.props.activeSvg.style.fontSize = (offsetY - this.props.activeSvg.attr.y) * 0.5;
                        } else {
                            this.props.activeSvg.style.fontSize = (offsetX - this.props.activeSvg.attr.x) * 0.5;
                        }

                        // Update svg
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;

            case 'circle':
                switch (this.props.toolState) {
                    case 'drawing':
                        // Test whichever is bigger and apply to circle radius
                        if (offsetY - this.props.activeSvg.attr.cy > offsetX - this.props.activeSvg.attr.cx) {
                            this.props.activeSvg.attr.r = offsetY - this.props.activeSvg.attr.cy;
                        } else {
                            this.props.activeSvg.attr.r = offsetX - this.props.activeSvg.attr.cx;
                        }

                        // Update svg
                        this.props.updateSvg(this.props.lastSvgIndex, this.props.activeSvg);
                        break;
                }
                break;
        }

        // Cache move event only if there are any svgs selected
        if (this.props.selectedSvgIndexes.length > 0) {
            // Cache mouse move event
            event.persist();
            this.props.setMouseMoveEvent(event);
        }
    }

    handleKeyDown(event) {
        // Check if actions modal is not opened or input field is in focus and don't apply custom shortcuts
        if (!_.isNull(document.getElementById('edit-action-modal')) || _.includes(['TEXTAREA', 'INPUT'], event.target.tagName)) {
            return;
        }

        // Backspace|Delete => delete object
        if (event.keyCode == 8 || event.keyCode == 46) {
            // Check if input is not in focus
            if (!_.includes(['TEXTAREA', 'INPUT'], event.target.tagName)) {
                event.preventDefault();
                this.props.removeSvg(this.props.lastSvgIndex);
            }
        }

        // ESC => deactivate editable
        if (event.keyCode == 27) {
            event.preventDefault();
            this.props.clearSvgSelection();
        }

        // Shift key => set keyMap state
        if (event.keyCode == 16) {
            event.preventDefault();
            this.props.keyMapSetKey('shift', true);
        }

        // Ctrl/CMD key => set keyMap state
        if (event.keyCode == KeyHelpers.getCtrlKeyCode()) {
            event.preventDefault();
            this.props.keyMapSetKey('ctrl', true);
        }

        // Ctrl + S => save
        if (this.props.keyMap.ctrl && event.keyCode == 83) {
            this.props.editMap(this.props.map);
            event.preventDefault();
        }

        // Ctrl + C => copy
        if (this.props.keyMap.ctrl && event.keyCode == 67) {
            this.props.clipboardCopy();
            event.preventDefault();
        }

        // Ctrl + V => paste
        if (this.props.keyMap.ctrl && event.keyCode == 86) {
            // Clear selection before pasting
            this.props.clearSvgSelection();
            this.props.clipboardPaste();
            event.preventDefault();
        }

        // Ctrl + X => cut
        if (this.props.keyMap.ctrl && event.keyCode == 88) {
            this.props.clipboardCut();
            event.preventDefault();
        }
    }

    handleKeyUp(event) {
        // Shift key => set keyMap state
        if (event.keyCode == 16) {
            event.preventDefault();
            this.props.keyMapSetKey('shift', false);
        }

        // Ctrl/CMD key => set keyMap state
        if (event.keyCode == KeyHelpers.getCtrlKeyCode()) {
            event.preventDefault();
            this.props.keyMapSetKey('ctrl', false);
        }
    }

    render() {
        return (
            <svg
                onClick={(event) => this.handleCanvasClick(event)}
                onDoubleClick={(event) => this.handleCanvasDoubleClick(event)}
                onMouseMove={(event) => this.handleCanvasMouseMove(event)}
                onMouseDown={(event) => this.handleCanvasMouseDown(event)}
                onMouseUp={(event) => this.handleCanvasMouseUp(event)}
                style={{
                    width: '100%', height: '600px', background: '#fff',
                    backgroundSize: `${this.props.grid.size}px ${this.props.grid.size}px, ${this.props.grid.size}px ${this.props.grid.size}px`,
                    backgroundImage: 'linear-gradient(#f1f1f1 1px, transparent 1px), linear-gradient(90deg, #f1f1f1 1px, transparent 1px)',
                    backgroundPosition: '-1px -1px, -1px -1px'
                }}
            >
                {_.map(this.props.mapData.svg, (item, index) => {
                    switch (item.type) {
                        case 'rect':
                            return <Rect
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'circle':
                            return <Circle
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'ellipse':
                            return <Ellipse
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'line':
                            return <Line
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'path':
                            return <Path
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'text':
                            return <Text
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        case 'position':
                            return <Position
                                onUpdateSvg={(i, s) => this.props.updateSvg(i, s)}
                                eventMouseMove={this.props.eventMouseMove}
                                grid={this.props.grid}
                                key={index}
                                index={index}
                                item={item}
                            />;

                        default:
                            return;
                    }
                })}
            </svg>
        );
    }
}

Canvas.propTypes = {
    tool: React.PropTypes.string,
    toolState: React.PropTypes.string,
    mapData: React.PropTypes.object,
    activeSvg: React.PropTypes.object,
    lastSvgIndex: React.PropTypes.number,
    eventMouseMove: React.PropTypes.object,
    selectedSvgIndexes: React.PropTypes.array,
    keyMap: React.PropTypes.object,
    styles: React.PropTypes.object,
    grid: React.PropTypes.object,
    map: React.PropTypes.object,

    // Functions
    editMap: React.PropTypes.func,
    addSvg: React.PropTypes.func,
    updateSvg: React.PropTypes.func,
    setToolState: React.PropTypes.func,
    setMouseMoveEvent: React.PropTypes.func,
    removeSvg: React.PropTypes.func,
    keyMapSetKey: React.PropTypes.func,
    setSvgSelection: React.PropTypes.func,
    addSvgToSelection: React.PropTypes.func,
    removeSvgFromSelection: React.PropTypes.func,
    clearSvgSelection: React.PropTypes.func,
    clipboardCopy: React.PropTypes.func,
    clipboardCut: React.PropTypes.func,
    clipboardPaste: React.PropTypes.func
};

export default connect(
    canvasSelector,
    (dispatch) => {
        return bindActionCreators({ addSvg, updateSvg, setToolState, setMouseMoveEvent, removeSvg, keyMapSetKey, editMap,
            setSvgSelection, addSvgToSelection, removeSvgFromSelection, clearSvgSelection, clipboardCopy, clipboardCut, clipboardPaste }, dispatch);
    }
)(Canvas)
