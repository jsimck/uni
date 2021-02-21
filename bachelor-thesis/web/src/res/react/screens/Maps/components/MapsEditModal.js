import React, { Component } from 'react'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { addMap, editMap } from '../actions/maps'
import { mapsEditModalSelector } from '../selectors/mapsEditModal'
import { hideMapsEditModal } from '../actions/mapsEditModal'
import { Modal, ModalBody, ModalFooter, ModalTitle, ModalHeader, Input, Button } from 'react-bootstrap'
import { pushNotification } from '../../../actions/notifications'
import { ButtonLoading } from '../../../components'

class MapsEditModal extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: '',
            description: '',
            titleInvalid: false
        }
    }

    resetModalState() {
        this.setState({
            title: !_.isEmpty(this.props.map) ? this.props.map.title : '',
            description: !_.isEmpty(this.props.map) ? this.props.map.description : ''
        });
    }

    linkState(e, key) {
        this.setState({
            [key]: e.target.value
        });
    }

    validateState() {
        // Make validations
        this.setState({
            titleInvalid: this.state.title.length <= 0
        });

        // Check if state is invalid
        if (this.state.title.length <= 0) {
            // Push notification
            this.props.pushNotification('danger', 'Název mapy musí být vyplněn');
            return false;
        }

        // State is valid return true
        return true;
    }

    updateMap() {
        if (!this.validateState()) {
            return;
        }

        if (!_.isEmpty(this.props.map)) {
            // Update Existing Map
            this.props.editMap({
                ...this.props.map,
                title: this.state.title,
                description: this.state.description
            });
        } else {
            // Create new map
            this.props.addMap(this.state.title, this.state.description);
        }
    }

    render() {
        return (
            <Modal
                onEnter={() => this.resetModalState()}
                className="modal-primary"
                show={this.props.isVisible}
            >
                <ModalHeader>
                    <ModalTitle>
                        {this.props.map ? 'Editovat' : 'Vytvořit'} mapu
                    </ModalTitle>
                </ModalHeader>

                <ModalBody>
                    <Input
                        type="text"
                        value={this.state.title}
                        onChange={(e) => this.linkState(e, 'title')}
                        bsStyle={this.state.titleInvalid ? 'error' : null}
                        label="Název:"
                    />
                    <Input
                        type="textarea"
                        rows={10}
                        value={this.state.description}
                        onChange={(e) => this.linkState(e, 'description')}
                        label="Popis:"
                    />
                </ModalBody>

                <ModalFooter>
                    <ButtonLoading
                        onClick={() => this.updateMap()}
                        loading={this.props.processing}
                        disabled={this.props.processing}
                        bsStyle="primary"
                    >
                        {this.props.map ? 'Editovat' : 'Vytvořit'} mapu
                    </ButtonLoading>
                    <Button onClick={() => this.props.hideMapsEditModal()}>Zavřít</Button>
                </ModalFooter>
            </Modal>
        );
    }
}

MapsEditModal.propTypes = {
    map: React.PropTypes.object,
    isVisible: React.PropTypes.bool,
    processing: React.PropTypes.bool,

    // Functions
    addMap: React.PropTypes.func,
    hideMapsEditModal: React.PropTypes.func,
    pushNotification: React.PropTypes.func,
    editMap: React.PropTypes.func
};

export default connect(
    mapsEditModalSelector,
    (dispatch) => {
        return bindActionCreators({ addMap, hideMapsEditModal, editMap, pushNotification }, dispatch);
    }
)(MapsEditModal)