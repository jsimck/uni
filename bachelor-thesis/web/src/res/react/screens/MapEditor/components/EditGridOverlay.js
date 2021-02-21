import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Overlay, Input, Button, Row, Col } from 'react-bootstrap'
import { editGridOverlaySelector } from '../selectors/editGridOverlay'
import { updateGrid } from '../actions/canvas'

class EditGridOverlay extends Component {
    constructor(props) {
        super(props);

        this.state = {
            enabled: props.grid.enabled,
            size: props.grid.size
        };
    }

    componentWillReceiveProps(nextProps) {
        if (nextProps.show) {
            this.setState({
                enabled: nextProps.grid.enabled,
                size: nextProps.grid.size
            });
        }
    }

    linkState(key, value) {
        this.setState({
            [key]: value
        }, () => {
            // Update Grid
            this.props.updateGrid({
                enabled: this.state.enabled,
                size: this.state.size
            });
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
                <div className="overlay-popover" style={{width: 200, marginLeft: 40}}>
                    <Input
                        onChange={(event) => this.linkState('size', parseInt(event.target.value) || 0)}
                        label="Velikost mřížky:"
                        value={this.state.size}
                        type="text"
                    />

                    <div className="checkbox check-primary" style={{marginTop: -5}}>
                        <input
                            onChange={() => this.linkState('enabled', !this.state.enabled)}
                            checked={this.state.enabled}
                            id="edit-grid-enable"
                            type="checkbox"
                            value="1"
                        />
                        <label htmlFor="edit-grid-enable">
                            Zapnout přichycení
                        </label>
                    </div>
                </div>
            </Overlay>
        );
    }
}

EditGridOverlay.propTypes = {
    grid: React.PropTypes.object,
    show: React.PropTypes.bool,

    // Functions
    updateGrid: React.PropTypes.func
};

export default connect(
    editGridOverlaySelector,
    (dispatch) => {
        return bindActionCreators({ updateGrid }, dispatch);
    }
)(EditGridOverlay)