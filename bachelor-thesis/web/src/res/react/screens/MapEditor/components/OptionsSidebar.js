import React, { Component } from 'react'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import classNames from 'classnames';
import ColorPicker from 'react-color';
import { Row, Col, Input, Button, OverlayTrigger, Tooltip } from 'react-bootstrap'
import { OptionsSidebarBlock, OptionsSidebarBody, OptionsSidebarHeader, OptionsSidebarHelpBlock } from './optionsSidebar/index'
import { Confirm } from '../../../components'
import { editMap } from '../../Maps/actions/maps'
import { updateSvg, removeSvgAction } from '../actions/canvas'
import { showEditActionModal } from '../actions/editActionModal'
import { openQrModal } from '../../../actions/qrModal'
import { pushNotification } from '../../../actions/notifications'
import { optionsSidebarSelector } from '../selectors/optionsSidebar'

class OptionsSidebar extends Component {
    constructor(props) {
        super(props);

        this.state = {
            type: props.type,
            x: 0,
            y: 0,
            width: 0,
            height: 0,
            fontSize: 0,
            content: '',
            fill: 'rgba(0, 0, 0, 1)',
            stroke: 'rgba(0, 0, 0, 1)',
            strokeWidth: 0,
            strokeRadius: 0,
            strokeLinecap: 'square',
            blending: 'normal',
            toggled: false,
            action: null,
            confirmDeleteAction: false,
            colorPickerVisible: false,
            currentColorValue: '',
            currentColor: '',
            eventId: 0
        };
    }

    setDefaultStateEventId(nextProps) {
        // Set default state id
        var eventId = 0;
        if (!_.isUndefined(nextProps.events[0])) {
            eventId = nextProps.events[0].id;
        }

        // Check if previous id still exists and assign it
        if (!_.isNull(nextProps.svg.data.eventId) && !_.isUndefined(_.find(nextProps.events, {id: nextProps.svg.data.eventId}))) {
            eventId = nextProps.svg.data.eventId;
        }

        // Set Event ID
        this.setState({
            eventId: eventId
        });
    }

    componentWillReceiveProps(nextProps) {
        if (!_.isEmpty(nextProps.svg)) {
            // Set state values to proper keys
            switch (nextProps.svg.type) {
                case 'rect':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.x,
                        y: nextProps.svg.attr.y,
                        width: nextProps.svg.attr.width,
                        height: nextProps.svg.attr.height,
                        strokeRadius: nextProps.svg.attr.rx,
                        fill: nextProps.svg.style.fill,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'text':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.x,
                        y: nextProps.svg.attr.y,
                        fill: nextProps.svg.style.fill,
                        fontSize: nextProps.svg.style.fontSize,
                        content: nextProps.svg.data.content,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'ellipse':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.cx,
                        y: nextProps.svg.attr.cy,
                        width: nextProps.svg.attr.rx,
                        height: nextProps.svg.attr.ry,
                        fill: nextProps.svg.style.fill,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'circle':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.cx,
                        y: nextProps.svg.attr.cy,
                        width: nextProps.svg.attr.r,
                        fill: nextProps.svg.style.fill,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'line':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.x1,
                        y: nextProps.svg.attr.y1,
                        width: nextProps.svg.attr.x2,
                        height: nextProps.svg.attr.y2,
                        strokeLinecap: nextProps.svg.attr.strokeLinecap,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'path':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        fill: nextProps.svg.style.fill,
                        stroke: nextProps.svg.style.stroke,
                        strokeWidth: nextProps.svg.style.strokeWidth,
                        blending: nextProps.svg.style.mixBlendMode,
                        transform: nextProps.svg.style.transform
                    });
                    break;

                case 'position':
                    this.setState({
                        type: nextProps.svg.type,
                        action: nextProps.svg.action,
                        x: nextProps.svg.attr.x,
                        y: nextProps.svg.attr.y
                    });
                    break;
            }

            // Set default selected event for actions and positions
            this.setDefaultStateEventId(nextProps);
        }
    }

    getInputLabel(label) {
        var type = _.includes(['ellipse', 'circle', 'line'], this.state.type) ? this.state.type : 'other';
        var labels = {
            other: {
                x: 'x',
                y: 'y',
                width: 'šířka',
                height: 'výška'
            },
            ellipse: {
                x: 'cx',
                y: 'cy',
                width: 'poloměr x',
                height: 'poloměr y'
            },
            circle: {
                x: 'cx',
                y: 'cy',
                width: 'poloměr'
            },
            line: {
                x: 'x1',
                y: 'y1',
                width: 'x2',
                height: 'y2'
            }
        };

        // Return labels based on type
        return labels[type][label];
    }

    handleShowColorPicker(color) {
        this.setState({
            colorPickerVisible: true,
            currentColor: color,
            currentColorValue: this.state[color]
        });
    }

    handleCloseColorPicker(colors) {
        // Reset state
        this.setState({
            colorPickerVisible: false,
            [this.state.currentColor]: `rgba(${colors.rgb.r}, ${colors.rgb.g}, ${colors.rgb.b}, ${colors.rgb.a})`,
            currentColor: '',
            currentColorValue: ''
        }, () => {
            // Update props
            this.handleSvgUpdate();
        });
    }

    handleSvgUpdate() {
        // Get svg and set values
        var svg = {};

        // Set state values to proper keys
        switch (this.state.type) {
            case 'rect':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        x: parseInt(this.state.x) || 0,
                        y: parseInt(this.state.y) || 0,
                        width: parseInt(this.state.width) || 0,
                        height: parseInt(this.state.height) || 0,
                        rx: parseInt(this.state.strokeRadius) || 0,
                        ry: parseInt(this.state.strokeRadius) || 0,
                    },
                    style: {
                        ...this.props.svg.style,
                        fill: this.state.fill,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;

            case 'text':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        x: parseInt(this.state.x) || 0,
                        y: parseInt(this.state.y) || 0
                    },
                    data: {
                        ...this.props.svg.data,
                        content: this.state.content,
                        eventId: this.state.eventId
                    },
                    style: {
                        ...this.props.svg.style,
                        fontSize: this.state.fontSize,
                        fill: this.state.fill,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    }
                };
                break;

            case 'ellipse':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        cx: parseInt(this.state.x) || 0,
                        cy: parseInt(this.state.y) || 0,
                        rx: parseInt(this.state.width) || 0,
                        ry: parseInt(this.state.height) || 0
                    },
                    style: {
                        ...this.props.svg.style,
                        fill: this.state.fill,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;

            case 'circle':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        cx: parseInt(this.state.x) || 0,
                        cy: parseInt(this.state.y) || 0,
                        r: parseInt(this.state.width) || 0
                    },
                    style: {
                        ...this.props.svg.style,
                        fill: this.state.fill,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;

            case 'line':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        x1: parseInt(this.state.x) || 0,
                        y1: parseInt(this.state.y) || 0,
                        x2: parseInt(this.state.width) || 0,
                        y2: parseInt(this.state.height) || 0,
                        strokeLinecap: this.state.strokeLinecap
                    },
                    style: {
                        ...this.props.svg.style,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;

            case 'path':
                svg = {
                    ...this.props.svg,
                    style: {
                        ...this.props.svg.style,
                        fill: this.state.fill,
                        stroke: this.state.stroke,
                        strokeWidth: parseInt(this.state.strokeWidth) || 0,
                        mixBlendMode: this.state.blending,
                        transform: this.state.transform
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;

            case 'position':
                svg = {
                    ...this.props.svg,
                    attr: {
                        ...this.props.svg.attr,
                        x: parseInt(this.state.x) || 0,
                        y: parseInt(this.state.y) || 0
                    },
                    data: {
                        ...this.props.svg.data,
                        eventId: this.state.eventId
                    }
                };
                break;
        }

        // Update svg with new values
        this.props.updateSvg(this.props.selectedSvgIndexes[0], svg);
    }

    handleOnChange(event, key, parse = false) {
        // If parse is defined, parse to number
        var parsedValue = parse ? parseInt(event.target.value) : event.target.value;

        // If it's transform, create rotate function from number
        if (key == 'transform') {
            parsedValue = `rotate(${parsedValue}deg)`;
        }

        // Update everything else
        this.setState({
            [key]: parsedValue
        }, () => {
            // Update svg props
            this.handleSvgUpdate();
        });
    }

    handleOnToggleClick() {
        // Update state
        this.setState({
            toggled: !this.state.toggled
        });
    }

    handleOnDeleteAction() {
        this.setState({
            confirmDeleteAction: true
        });
    }

    handleOnDeleteActionConfirm(confirm) {
        if (confirm) {
            // Update svg
            this.props.removeSvgAction(this.props.selectedSvgIndexes[0]);

            // Save map after 50ms timeout so the changes takes effect
            // The change is almost immediate, it would work even with 10ms delay,
            // but the 50ms is there just to make sure everything is applied
            setTimeout(() => {
                // Save updated map
                this.props.editMap(this.props.map);
            }, 50);
        }

        // Hide Confirm Modal
        this.setState({
            confirmDeleteAction: false
        });
    }

    parseDegFromTransform(transform) {
        return transform.replace(/[^\d\.\-\s]/g, '');
    }

    handleShowQrCode() {
        var qrAction = {};

        // Create action based on type
        if (this.state.type == 'position') {
            qrAction = {
                type: 'position',
                data: {
                    eventId: this.state.eventId,
                    x: this.state.x,
                    y: this.state.y
                }
            }
        } else {
            qrAction = {
                type: 'action',
                data: {
                    eventId: this.state.eventId,
                    mapId: this.props.map.id,
                    actionId: this.state.action.id
                }
            }
        }

        this.props.openQrModal(JSON.stringify(qrAction));
    }

    render() {
        var sidebar = null;

        // Render options builder if only one svg is selected
        if (this.props.selectedSvgIndexes.length == 1) {
            // Delete action tooltip
            const tooltipDelete = <Tooltip id="edit-action-modal-delete">Smazat Akci</Tooltip>;

            // Create sidebar markup
            sidebar = (
                <div id="options-builder" className={classNames({'toggled': this.state.toggled})}>
                    <div onClick={() => this.handleOnToggleClick()} id="options-builder-handler">
                        <i className={classNames('fa', {'fa-angle-left': this.state.toggled}, {'fa-angle-right': !this.state.toggled})}></i>
                    </div>

                    {!this.state.toggled &&
                        <div id="options-builder-content" className="options-builder-content">
                            <OptionsSidebarBlock show={this.state.type != 'path'}>
                                <OptionsSidebarHeader>
                                    Pozice:
                                </OptionsSidebarHeader>

                                <OptionsSidebarBody>
                                    <Col xs={6}>
                                        <Input
                                            type="text"
                                            onChange={(event) => this.handleOnChange(event, 'x', true)}
                                            value={this.state.x}
                                            label={this.getInputLabel('x') + ':'}
                                            bsSize="small"
                                        />
                                    </Col>
                                    <Col xs={6}>
                                        <Input
                                            type="text"
                                            onChange={(event) => this.handleOnChange(event, 'y', true)}
                                            value={this.state.y}
                                            label={this.getInputLabel('y') + ':'}
                                            bsSize="small"
                                        />
                                    </Col>

                                    {!_.includes(['text', 'position'], this.state.type) &&
                                        <div>
                                            <Col xs={6}>
                                                <Input
                                                    type="text"
                                                    onChange={(event) => this.handleOnChange(event, 'width', true)}
                                                    value={this.state.width}
                                                    label={this.getInputLabel('width') + ':'}
                                                    bsSize="small"
                                                />
                                            </Col>

                                            {this.state.type != 'circle' &&
                                                <Col xs={6}>
                                                    <Input
                                                        type="text"
                                                        onChange={(event) => this.handleOnChange(event, 'height', true)}
                                                        value={this.state.height}
                                                        label={this.getInputLabel('height') + ':'}
                                                        bsSize="small"
                                                    />
                                                </Col>
                                            }
                                        </div>
                                    }
                                </OptionsSidebarBody>
                            </OptionsSidebarBlock>

                            <OptionsSidebarBlock show={this.state.type != 'position'}>
                                <OptionsSidebarHeader>
                                    Styl:
                                </OptionsSidebarHeader>

                                <OptionsSidebarBody>
                                    {this.state.type != 'line' &&
                                        <Col xs={6}>
                                            <Input
                                                type="text" style={{background: this.state.fill, color: 'transparent', cursor: 'pointer'}}
                                                onClick={() => this.handleShowColorPicker('fill')}
                                                value={this.state.fill}
                                                label="Vyplnění:"
                                                bsSize="small"
                                                readOnly
                                            />
                                        </Col>
                                    }

                                    <Col xs={6}>
                                        <Input
                                            type="text"
                                            style={{background: this.state.stroke, color: 'transparent', cursor: 'pointer'}}
                                            onClick={() => this.handleShowColorPicker('stroke')}
                                            value={this.state.stroke}
                                            label="Čára:"
                                            bsSize="small"
                                            readOnly
                                        />
                                    </Col>
                                    <Col xs={6}>
                                        <Input
                                            type="text"
                                            onChange={(event) => this.handleOnChange(event, 'strokeWidth', true)}
                                            value={this.state.strokeWidth}
                                            label="Šířka čáry:"
                                            bsSize="small"
                                        />
                                    </Col>

                                    {this.state.type == 'rect' &&
                                        <Col xs={6}>
                                            <Input
                                                type="text"
                                                onChange={(event) => this.handleOnChange(event, 'strokeRadius', true)}
                                                value={this.state.strokeRadius}
                                                label="Poloměr čáry:"
                                                bsSize="small"
                                            />
                                        </Col>
                                    }

                                    {_.includes(['ellipse', 'rect', 'text'], this.state.type) &&
                                        <Col xs={6}>
                                            <Input
                                                type="text"
                                                onChange={(event) => this.handleOnChange(event, 'transform', true)}
                                                value={this.parseDegFromTransform(this.state.transform)}
                                                label="Rotace (stupně):"
                                                bsSize="small"
                                            />
                                        </Col>
                                    }

                                    {this.state.type == 'line' &&
                                        <Col xs={6}>
                                            <div className="pg-select">
                                                <Input
                                                    onChange={(event) => this.handleOnChange(event, 'strokeLinecap')}
                                                    defaultValue={this.state.strokeLinecap}
                                                    type="select"
                                                    label="Zakončení čáry:"
                                                >
                                                    <option value="square">square</option>
                                                    <option value="round">round</option>
                                                    <option value="butt">butt</option>
                                                </Input>
                                            </div>
                                        </Col>
                                    }

                                    <Col xs={6}>
                                        <div className="pg-select">
                                            <Input
                                                onChange={(event) => this.handleOnChange(event, 'blending')}
                                                defaultValue={this.state.blending}
                                                type="select"
                                                label="Míšení:"
                                            >
                                                <option value="normal">normal</option>
                                                <option value="multiply">multiply</option>
                                                <option value="screen">screen</option>
                                                <option value="overlay">overlay</option>
                                                <option value="darken">darken</option>
                                                <option value="lighten">lighten</option>
                                                <option value="color-dodge">color-dodge</option>
                                                <option value="color-burn">color-burn</option>
                                                <option value="hard-light">hard-light</option>
                                                <option value="soft-light">soft-light</option>
                                                <option value="difference">difference</option>
                                                <option value="exclusion">exclusion</option>
                                                <option value="hue">hue</option>
                                                <option value="saturation">saturation</option>
                                                <option value="color">color</option>
                                                <option value="luminosity">luminosity</option>
                                            </Input>
                                        </div>
                                    </Col>

                                    {this.state.type == 'text' &&
                                        <Col xs={6}>
                                            <Input
                                                type="text"
                                                onChange={(event) => this.handleOnChange(event, 'fontSize')}
                                                value={this.state.fontSize} label="Font Size:" bsSize="small"
                                            />
                                        </Col>
                                    }

                                    {this.state.type == 'text' &&
                                        <Col xs={12}>
                                            <Input
                                                type="textarea"
                                                onChange={(event) => this.handleOnChange(event, 'content')}
                                                value={this.state.content} label="Content:"
                                            />
                                        </Col>
                                    }

                                    <ColorPicker
                                        positionCSS={{position: 'absolute', top: '-60px', left: '-8px'}}
                                        color={this.state.currentColorValue}
                                        display={this.state.colorPickerVisible}
                                        onClose={(colors) => this.handleCloseColorPicker(colors)}
                                        type="chrome"
                                    />
                                </OptionsSidebarBody>
                            </OptionsSidebarBlock>

                            <OptionsSidebarBlock show={this.state.type != 'position'}>
                                <OptionsSidebarHeader>
                                    Akce:
                                </OptionsSidebarHeader>

                                <OptionsSidebarBody>
                                    <Col xs={12}>
                                        <div className="form-group m-t-6 form-group-sm">
                                            <OptionsSidebarHelpBlock>
                                                Zde můžete přiřadit vlastní akce, které se zobrazí po kliknutí na tento SVG objekt.
                                            </OptionsSidebarHelpBlock>

                                            <Button
                                                onClick={() => this.props.showEditActionModal(this.props.selectedSvgIndexes[0], this.props.svg)}
                                                bsStyle="primary"
                                                bsSize="xsmall"
                                            >
                                                <i className="pg pg-plus"></i> {(this.props.svg.action.hasOwnProperty('title')) ? 'Editovat' : 'Vytvořit'} akci
                                            </Button>

                                            {this.props.svg.action.hasOwnProperty('title') &&
                                                <OverlayTrigger overlay={tooltipDelete} placement="top">
                                                    <Button
                                                        onClick={() => this.handleOnDeleteAction()}
                                                        bsStyle="danger"
                                                        className="m-l-5"
                                                        bsSize="xsmall"
                                                    >
                                                        <i className="fa fa-trash"></i>
                                                    </Button>
                                                </OverlayTrigger>
                                            }
                                        </div>
                                    </Col>
                                </OptionsSidebarBody>
                            </OptionsSidebarBlock>

                            <OptionsSidebarBlock show={this.props.events.length > 0 && (this.state.type == 'position' || this.state.action.hasOwnProperty('title'))}>
                                <OptionsSidebarHeader>
                                    QR Code:
                                </OptionsSidebarHeader>

                                <OptionsSidebarBody>
                                    <Col xs={12}>
                                        <div className="pg-select">
                                            <Input
                                                onChange={(event) => this.handleOnChange(event, 'eventId', true)}
                                                value={this.state.eventId}
                                                type="select"
                                                label="Vybrat událost:"
                                            >
                                                {this.props.events.map((event) => {
                                                    return <option key={event.id} value={event.id}>{event.title}</option>;
                                                })}
                                            </Input>
                                        </div>

                                        <OptionsSidebarHelpBlock style={{marginTop: 0}}>
                                            Generovat QR kód, pro zobrazení této {this.state.action.hasOwnProperty('title') ? 'akce' : 'pozice'} po jeho načtení v mobilní aplikaci. Uživatel bude přesunut na mapu vybrané události.
                                        </OptionsSidebarHelpBlock>

                                        <Button
                                            onClick={() => this.handleShowQrCode()}
                                            bsStyle="primary"
                                            bsSize="xsmall"
                                        >
                                            <i className="fa fa-qrcode"></i> Generovat QR kód
                                        </Button>

                                        {this.props.events.length <= 0 &&
                                            <OptionsSidebarHelpBlock>
                                                Před generováním QR kódů musíte nejdříve nějaké události přiřadit tuto mapu.
                                            </OptionsSidebarHelpBlock>
                                        }
                                    </Col>
                                </OptionsSidebarBody>
                            </OptionsSidebarBlock>
                        </div>
                    }

                    <Confirm
                        show={this.state.confirmDeleteAction}
                        title="Smazat Akci"
                        onConfirm={(confirm) => this.handleOnDeleteActionConfirm(confirm)}
                    >
                        Opravdu chcete smazat tuto akci?
                    </Confirm>
                </div>
            );
        }

        // Render sidebar
        return sidebar;
    }
}

OptionsSidebar.propTypes = {
    selectedSvgIndexes: React.PropTypes.array,
    events: React.PropTypes.array,
    svg: React.PropTypes.object,
    map: React.PropTypes.object,

    // Functions
    editMap: React.PropTypes.func,
    updateSvg: React.PropTypes.func,
    removeSvgAction: React.PropTypes.func,
    showEditActionModal: React.PropTypes.func,
    pushNotification: React.PropTypes.func,
    openQrModal: React.PropTypes.func
};

export default connect(
    optionsSidebarSelector,
    (dispatch) => {
        return bindActionCreators({ editMap, updateSvg, removeSvgAction, showEditActionModal, pushNotification, openQrModal }, dispatch);
    }
)(OptionsSidebar)
