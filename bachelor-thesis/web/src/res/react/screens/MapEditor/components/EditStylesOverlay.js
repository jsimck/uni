import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import ColorPicker from 'react-color'
import { Overlay, Input, Button, Row, Col } from 'react-bootstrap'
import { updateGlobalStyles } from '../actions/canvas'
import { editStylesOverlaySelector } from '../selectors/editStylesOverlay'

class EditStylesOverlay extends Component {
    constructor(props) {
        super(props);

        this.state = {
            colorPickerVisible: false,
            currentColor: '',
            currentColorValue: '',
            fill: '#626262',
            stroke: '#10cfbd',
            mixBlendMode: 'normal',
            strokeWidth: 4
        };
    }

    componentWillReceiveProps(nextProps) {
        if (nextProps.show) {
            this.setState({
                fill: nextProps.styles.fill,
                stroke: nextProps.styles.stroke,
                mixBlendMode: nextProps.styles.mixBlendMode,
                strokeWidth: nextProps.styles.strokeWidth
            });
        }
    }

    handleShowColorPicker(color) {
        this.setState({
            colorPickerVisible: true,
            currentColor: color,
            currentColorValue: this.state[color]
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

    linkState(key, value) {
        this.setState({
            [key]: value
        }, () => {
            // Update global styles
            this.props.updateGlobalStyles({
                fill: this.state.fill,
                stroke: this.state.stroke,
                mixBlendMode: this.state.mixBlendMode,
                strokeWidth: this.state.strokeWidth
            })
        });
    }

    render() {
        return (
            <Overlay
                show={this.props.show}
                onHide={() => this.props.onClose()}
                placement="bottom"
                animation={false}
                container={this.props.container}
                rootClose={true}
            >
                <div className="overlay-popover" style={{width: 300}}>
                    <Row>
                        <Col xs={6}>
                            <Input
                                label="Vyplnění:"
                                type="text"
                                className="no-user-select"
                                style={{background: this.state.fill, color: this.state.fill, cursor: 'pointer'}}
                                onChange={(event) => this.linkState('fill', event.target.value)}
                                onClick={() => this.handleShowColorPicker('fill')}
                                value={this.state.fill}
                            />
                        </Col>
                        <Col xs={6}>
                            <Input
                                label="Barva čáry:"
                                type="text"
                                className="no-user-select"
                                style={{background: this.state.stroke, color: this.state.stroke, cursor: 'pointer'}}
                                onChange={(event) => this.linkState('stroke', event.target.value)}
                                onClick={() => this.handleShowColorPicker('stroke')}
                                value={this.state.stroke}
                            />
                        </Col>
                    </Row>
                    <Row>
                        <Col xs={6}>
                            <div className="mapedit-select mapedit-select-fix">
                                <Input
                                    onChange={(event) => this.linkState('mixBlendMode', event.target.value)}
                                    value={this.state.mixBlendMode}
                                    label="Míšení:"
                                    type="select"
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
                        <Col xs={6}>
                            <Input
                                onChange={(event) => this.linkState('strokeWidth', event.target.value)}
                                value={this.state.strokeWidth}
                                label="Šířka čáry:"
                                type="text"
                            />
                        </Col>
                    </Row>

                    <ColorPicker
                        color={this.state.currentColorValue}
                        display={this.state.colorPickerVisible}
                        onClose={(colors) => this.handleCloseColorPicker(colors)}
                        type="chrome"
                    />
                </div>
            </Overlay>
        );
    }
}

EditStylesOverlay.propTypes = {
    styles: React.PropTypes.object,
    show: React.PropTypes.bool,

    // Functions
    updateGlobalStyles: React.PropTypes.func
};

export default connect(
    editStylesOverlaySelector,
    (dispatch) => {
        return bindActionCreators({ updateGlobalStyles }, dispatch);
    }
)(EditStylesOverlay)