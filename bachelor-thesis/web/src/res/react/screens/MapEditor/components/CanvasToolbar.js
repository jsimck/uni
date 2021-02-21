import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import classNames from 'classnames'
import { Button, ButtonGroup, Tooltip, OverlayTrigger, DropdownButton, MenuItem, Overlay, Input } from 'react-bootstrap'
import { ButtonLoading } from '../../../components'
import { editMap } from '../../Maps/actions/maps'
import { pushSvgBack, pushSvgBackward, pushSvgForward, pushSvgFront, clearSvgSelection, clipboardCopy, clipboardCut, clipboardPaste, removeSvg } from '../actions/canvas'
import { canvasToolbarSelector } from '../selectors/canvasToolbar'
import EditStylesOverlay from './EditStylesOverlay'
import EditGridOverlay from './EditGridOverlay'
import SVGFactory from '../helpers/SVGFactory'
import KeyHelpers from '../helpers/KeyHelpers'

class CanvasToolbar extends Component {
    constructor(props) {
        super(props);

        this.state = {
            defStylesModalVisible: false,
            gridOverlayVisible: false
        };
    }

    render() {
        // Create tooltips
        const tooltipPushBackward = <Tooltip id={'canvas-toolbar-action-push-backward'}>Posunout dozadu o jeden</Tooltip>;
        const tooltipPushForward = <Tooltip id={'canvas-toolbar-action-push-forward'}>Posunout dopředu o jeden</Tooltip>;
        const tooltipPushFront = <Tooltip id={'canvas-toolbar-action-push-front'}>Posunout dopředu</Tooltip>;
        const tooltipPushBack = <Tooltip id={'canvas-toolbar-action-push-back'}>Posunout dozadu</Tooltip>;
        const tooltipGrid = <Tooltip id={'canvas-toolbar-action-grid'}>Přichycení k mřížce</Tooltip>;
        const tooltipStyles = <Tooltip id={'canvas-toolbar-action-styles'}>Výchozí styly</Tooltip>;

        // Disable layer ordering if there's more then 1 item in selection or no item
        var layerOrderingDisabled = this.props.selectedSvgIndexes.length <= 0 || this.props.selectedSvgIndexes.length > 1;
        var svgSelected = this.props.selectedSvgIndexes.length <= 0;

        return (
            <div className="canvas-toolbar full-width">
                <ButtonGroup className="left-group">
                    <DropdownButton bsStyle="link" id="canvas-toolbar-dropdown-file" title="Úprava">
                        <MenuItem onClick={() => this.props.editMap(this.props.map)} disabled={this.props.processing}>
                            Uložit <kbd>{KeyHelpers.getCtrlKeyLabel()} + S</kbd>
                        </MenuItem>
                        <MenuItem onClick={() => location.reload()}>
                            Načíst znovu
                        </MenuItem>

                        <MenuItem divider />

                        <MenuItem disabled={svgSelected} onClick={() => this.props.clipboardCopy()}>
                            Kopírovat <kbd>{KeyHelpers.getCtrlKeyLabel()} + C</kbd>
                        </MenuItem>
                        <MenuItem disabled={svgSelected} onClick={() => this.props.clipboardCut()}>
                            Vyjmout <kbd>{KeyHelpers.getCtrlKeyLabel()} + X</kbd>
                        </MenuItem>
                        <MenuItem onClick={() => this.props.clipboardPaste()}>
                            Vložit <kbd>{KeyHelpers.getCtrlKeyLabel()} + V</kbd>
                        </MenuItem>

                        <MenuItem divider />

                        <MenuItem disabled={svgSelected} onClick={() => this.props.clearSvgSelection()}>
                            Zrušit výběr <kbd>ESC</kbd>
                        </MenuItem>
                        <MenuItem disabled={svgSelected} onClick={() => this.props.removeSvg()}>
                            Smazat vybrané <kbd><i className="fa fa-long-arrow-left"></i></kbd>
                        </MenuItem>
                    </DropdownButton>

                    <OverlayTrigger placement="top" overlay={tooltipStyles}>
                        <Button onClick={() => this.setState({ defStylesModalVisible: true })} bsStyle="link">
                            <i className="fa fa-magic"></i>
                        </Button>
                    </OverlayTrigger>

                    <EditStylesOverlay
                        onClose={() => this.setState({ defStylesModalVisible: false })}
                        show={this.state.defStylesModalVisible}
                        container={this}
                    />

                    <OverlayTrigger placement="top" overlay={tooltipGrid}>
                        <Button
                            onClick={() => this.setState({ gridOverlayVisible: true })}
                            className={classNames({'text-success': this.props.grid.enabled})}
                            bsStyle="link"
                        >
                            <i className="fa fa-th"></i>
                        </Button>
                    </OverlayTrigger>

                    <EditGridOverlay
                        onClose={() => this.setState({ gridOverlayVisible: false })}
                        show={this.state.gridOverlayVisible}
                        container={this}
                    />
                </ButtonGroup>

                <ButtonGroup className="right-group pull-right canvas-layer-ordering">
                    <OverlayTrigger overlay={tooltipPushFront} placement="top">
                        <Button
                            disabled={layerOrderingDisabled}
                            bsStyle="link"
                            onClick={() => this.props.pushSvgFront(this.props.firstSelectedSvgIndex)}
                        >
                            <span className="fa-stack">
                                <i className="fa fa-square"></i>
                                <i className="fa fa-square text-warning fa-layer-back"></i>
                            </span>
                        </Button>
                    </OverlayTrigger>
                    <OverlayTrigger overlay={tooltipPushBack} placement="top">
                        <Button
                            disabled={layerOrderingDisabled}
                            bsStyle="link"
                            onClick={() => this.props.pushSvgBack(this.props.firstSelectedSvgIndex)}
                        >
                            <span className="fa-stack">
                                <i className="fa fa-square fa-layer-back"></i>
                                <i className="fa fa-square text-warning"></i>
                            </span>
                        </Button>
                    </OverlayTrigger>
                    <OverlayTrigger overlay={tooltipPushForward} placement="top">
                        <Button
                            disabled={layerOrderingDisabled}
                            bsStyle="link"
                            onClick={() => this.props.pushSvgForward(this.props.firstSelectedSvgIndex)}
                        >
                            <span className="fa-stack">
                                <i className="fa fa-square"></i>
                                <i className="fa fa-square text-success fa-layer-back"></i>
                            </span>
                        </Button>
                    </OverlayTrigger>
                    <OverlayTrigger overlay={tooltipPushBackward} placement="top">
                        <Button
                            disabled={layerOrderingDisabled}
                            bsStyle="link"
                            onClick={() => this.props.pushSvgBackward(this.props.firstSelectedSvgIndex)}
                        >
                            <span className="fa-stack">
                                <i className="fa fa-square fa-layer-back"></i>
                                <i className="fa fa-square text-success"></i>
                            </span>
                        </Button>
                    </OverlayTrigger>
                </ButtonGroup>
            </div>
        );
    }
}

CanvasToolbar.propTypes = {
    map: React.PropTypes.object,
    grid: React.PropTypes.object,
    processing: React.PropTypes.bool,
    selectedSvgIndexes: React.PropTypes.array,
    firstSelectedSvgIndex: React.PropTypes.number,

    // Functions
    editMap: React.PropTypes.func,
    removeSvg: React.PropTypes.func,
    pushSvgBack: React.PropTypes.func,
    pushSvgBackward: React.PropTypes.func,
    pushSvgForward: React.PropTypes.func,
    pushSvgFront: React.PropTypes.func,
    clearSvgSelection: React.PropTypes.func,
    clipboardCopy: React.PropTypes.func,
    clipboardCut: React.PropTypes.func,
    clipboardPaste: React.PropTypes.func
};

export default connect(
    canvasToolbarSelector,
    (dispatch) => {
        return bindActionCreators({ editMap, pushSvgBack, pushSvgBackward, pushSvgForward, pushSvgFront, clearSvgSelection,
            clipboardCopy, clipboardCut, clipboardPaste, removeSvg}, dispatch);
    }
)(CanvasToolbar)