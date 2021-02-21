import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import ColorPicker from 'react-color'
import classNames from 'classnames'
import { Button, ButtonGroup, Tooltip, OverlayTrigger } from 'react-bootstrap'
import { setTool, setToolState } from '../actions/canvasTool'
import { clearSvgSelection, updateGlobalStyles } from '../actions/canvas'
import { canvasSidebarSelector } from '../selectors/canvasSidebar'
import SVGFactory from '../helpers/SVGFactory'

class CanvasSidebar extends Component {
    constructor(props) {
        super(props);

        this.state = {
            colorPickerVisible: false,
            currentColor: '',
            currentColorValue: ''
        };
    }

    handleShowColorPicker(color) {
        this.setState({
            colorPickerVisible: true,
            currentColor: color,
            currentColorValue: this.props.styles[color]
        });
    }

    handleCloseColorPicker(colors) {
        // Set color
        this.props.updateGlobalStyles({
            [this.state.currentColor]: `rgba(${colors.rgb.r}, ${colors.rgb.g}, ${colors.rgb.b}, ${colors.rgb.a})`
        });

        // Reset state
        this.setState({
            colorPickerVisible: false,
            currentColor: '',
            currentColorValue: ''
        });
    }

    handleToolClick(newTool, newState) {
        // Update tools if changed
        if (this.props.tool.length > 0 && this.props.tool == newTool) {
            this.props.setTool('');
            this.props.setToolState('');
        } else {
            this.props.setTool(newTool);
            this.props.setToolState(newState);
        }

        // Clear current selection
        this.props.clearSvgSelection();
    }

    render() {
        // Create tooltips
        const tooltipMove = <Tooltip id={'canvas-toolbar-action-move'}>Nástroj úprav</Tooltip>;
        const tooltipCircle = <Tooltip id={'canvas-toolbar-action-circle'}>Kruh</Tooltip>;
        const tooltipEllipse = <Tooltip id={'canvas-toolbar-action-ellipse'}>Elipsa</Tooltip>;
        const tooltipRect = <Tooltip id={'canvas-toolbar-action-rectangle'}>Obdelník</Tooltip>;
        const tooltipLine = <Tooltip id={'canvas-toolbar-action-line'}>Čára</Tooltip>;
        const tooltipPath = <Tooltip id={'canvas-toolbar-action-path'}>Cesta</Tooltip>;
        const tooltipText = <Tooltip id={'canvas-toolbar-action-text'}>Text</Tooltip>;
        const tooltipColor = <Tooltip id={'canvas-toolbar-action-color'}>Barvy vyplnění a čáry</Tooltip>;
        const tooltipPosition = <Tooltip id={'canvas-toolbar-action-position'}>Vytvořit pozici</Tooltip>;

        // Color picker CSS position
        const colorPickerPosition = {
            position: 'absolute',
            top: '60px',
            left: '0'
        };

        return (
            <div className="canvas-sidebar">
                <ButtonGroup vertical>
                    <OverlayTrigger placement="top" overlay={tooltipMove}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'move' })}
                            onClick={() => this.handleToolClick('move', 'active')}
                        >
                            <i className="fa fa-arrows"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipCircle}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'circle' })}
                            onClick={() => this.handleToolClick('circle', 'active')}
                        >
                            <i className="fa fa-circle"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipEllipse}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'ellipse' })}
                            onClick={() => this.handleToolClick('ellipse', 'active')}
                        >
                            <i className="fa fa-circle fa-ellipse"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipRect}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'rect' })}
                            onClick={() => this.handleToolClick('rect', 'active')}
                        >
                            <i className="fa fa-stop"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipLine}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'line' })}
                            onClick={() => this.handleToolClick('line', 'active')}
                        >
                            <i className="fa fa-minus"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipPath}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'path' })}
                            onClick={() => this.handleToolClick('path', 'active')}
                        >
                            <i className="fa fa-pencil"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipText}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'text' })}
                            onClick={() => this.handleToolClick('text', 'active')}
                        >
                            <i className="fa fa-font"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipPosition}>
                        <Button
                            bsStyle="link"
                            className={classNames({ 'tool-active': this.props.tool == 'position' })}
                            onClick={() => this.handleToolClick('position', 'active')}
                        >
                            <i className="fa fa-dot-circle-o"></i>
                        </Button>
                    </OverlayTrigger>

                    <OverlayTrigger placement="top" overlay={tooltipColor}>
                        <Button bsStyle="link" className="sidebar-color-picker">
                            <span className="fa-stack" style={{marginTop: '-5px'}}>
                                <i
                                    style={{color: this.props.styles.fill}}
                                    onClick={() => this.handleShowColorPicker('fill')}
                                    className="color-foreground fa fa-stop">
                                </i>
                                <i
                                    style={{color: this.props.styles.stroke}}
                                    onClick={() => this.handleShowColorPicker('stroke')}
                                    className="color-background fa fa-stop">
                                </i>
                            </span>
                        </Button>
                    </OverlayTrigger>
                </ButtonGroup>

                <ColorPicker
                    positionCSS={colorPickerPosition}
                    color={this.state.currentColorValue}
                    display={this.state.colorPickerVisible}
                    onClose={(colors) => this.handleCloseColorPicker(colors)}
                    type="chrome"
                />
            </div>
        );
    }
}

CanvasSidebar.propTypes = {
    tool: React.PropTypes.string,
    styles: React.PropTypes.object,

    // Redux methods
    setTool: React.PropTypes.func,
    setToolState: React.PropTypes.func,
    clearSvgSelection: React.PropTypes.func,
    updateGlobalStyles: React.PropTypes.func
};

export default connect(
    canvasSidebarSelector,
    (dispatch) => {
        return bindActionCreators({ setTool, setToolState, clearSvgSelection, updateGlobalStyles }, dispatch);
    }
)(CanvasSidebar)