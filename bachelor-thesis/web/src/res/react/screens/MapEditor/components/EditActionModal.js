import React, { Component } from 'react'
import _ from 'lodash'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { Modal, ModalBody, ModalFooter, ModalHeader, ModalTitle, Input, Button, Row, Col } from 'react-bootstrap'
import { RichEditor } from '../../../components'
import { editMap } from '../../Maps/actions/maps'
import { pushNotification } from '../../../actions/notifications'
import { hideEditActionModal } from '../actions/editActionModal'
import { updateSvgAction } from '../actions/canvas'
import { editActionModalSelector } from '../selectors/editActionModal'

class EditActionModal extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: '',
            content: null,
            titleInvalid: false
        };
    }

    linkState(value, key) {
        this.setState({
            [key]: value
        });
    };

    validateState() {
        // Make validations
        this.setState({
            titleInvalid: this.state.title.length <= 0
        });

        // Check if state is invalid
        if (this.state.title.length <= 0) {
            // Push notification
            this.props.pushNotification('danger', 'Název akce musí být vyplněn');
            return false;
        }

        // State is valid return true
        return true;
    }

    resetModalState() {
        this.setState({
            title: !_.isEmpty(this.props.svg.action.title) ? this.props.svg.action.title : '',
            content: !_.isEmpty(this.props.svg.action.content) ? this.props.svg.action.content : null
        });
    }

    handleOnUpdateAction() {
        if (!this.validateState()) {
            return;
        }

        // Update svg action
        this.props.updateSvgAction(this.props.index, this.state.title, this.state.content);

        // Hide modal
        this.props.hideEditActionModal();

        // Save map after 50ms timeout so the changes takes effect
        // The change is almost immediate, it would work even with 10ms delay,
        // but the 50ms is there just to make sure everything is applied
        setTimeout(() => {
            // Save updated map
            this.props.editMap(this.props.map);
        }, 50);
    }

    render() {
        // Don't call render if there's no svg selected
        if (this.props.index < 0) {
            return false;
        }

        return (
            <Modal
                onEnter={() => this.resetModalState()}
                id="edit-action-modal"
                className="modal-default event-modal"
                bsSize="lg"
                show={this.props.visible}
            >
                <ModalBody>
                    <div className="meta-wrapper">
                        <Input
                            className="title"
                            type="text"
                            value={this.state.title}
                            title={null}
                            style={{marginBottom: -5}}
                            placeholder="Název akce"
                            bsStyle={this.state.titleInvalid ? 'error' : null}
                            onChange={(e) => this.linkState(e.target.value, 'title')}
                        />
                    </div>
                    <div className="editor-wrapper">
                        <RichEditor
                            editorState={this.state.content}
                            initialEditorState={this.state.content}
                            onChange={(editorState) => this.linkState(editorState, 'content')}
                        />
                    </div>
                </ModalBody>

                <ModalFooter>
                    <Button onClick={() => this.handleOnUpdateAction()} bsStyle="success">
                        {((this.props.svg.action.hasOwnProperty('title'))) ? 'Editovat' : 'Vytvořit'} akci
                    </Button>
                    <Button onClick={() => this.props.hideEditActionModal()}>Zavřít</Button>
                </ModalFooter>
            </Modal>
        );
    }
}

EditActionModal.propTypes = {
    visible: React.PropTypes.bool,
    index: React.PropTypes.number,
    svg: React.PropTypes.object,
    map: React.PropTypes.object,

    // Functions
    updateSvgAction: React.PropTypes.func,
    hideEditActionModal: React.PropTypes.func,
    pushNotification: React.PropTypes.func,
    editMap: React.PropTypes.func
};

export default connect(
    editActionModalSelector,
    (dispatch) => {
        return bindActionCreators({ updateSvgAction, hideEditActionModal, editMap, pushNotification }, dispatch);
    }
)(EditActionModal)