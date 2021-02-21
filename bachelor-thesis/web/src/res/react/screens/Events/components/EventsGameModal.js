import React, { Component } from 'react'
import _ from 'lodash'
import classNames from 'classnames'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Modal, ModalBody, ModalHeader, ModalTitle, ModalFooter, Button, Input } from 'react-bootstrap'
import QRCode from 'qrcode.react'
import { updateEvent } from '../actions/events'
import { closeEventsGameModal } from '../actions/eventsGameModal'
import { pushNotification } from '../../../actions/notifications'
import { eventsGameModalSelector } from '../selectors/eventsGameModal'
import { RichEditor, Confirm } from '../../../components/'

class EventsGameModal extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: '',
            info: '',
            winningMessage: '',
            qrCount: 3,
            titleInvalid: false,
            infoInvalid: false,
            winningMessageInvalid: false,
            qrCountInvalid: false,
            deleteConfirmVisible: false
        }
    }

    linkState(key, value) {
        this.setState({
            [key]: value
        });
    }

    createFakeQrDownloadLink(index) {
        // Create invisible link to download qr code as img
        var downloadLink = document.createElement('a');
        downloadLink.href = document.querySelector('#gameQrCodesWrapper').children[index].children[0].toDataURL();
        downloadLink.download = 'game_qrcode_' + index;

        // Attach to dom and remove after click
        document.body.appendChild(downloadLink);
        downloadLink.click();
        document.body.removeChild(downloadLink);
    }

    handleOnDownloadClick(qrIndexes) {
        // If is array, download each image separately
        if (_.isArray(qrIndexes)) {
            _.map(qrIndexes, (index) => {
                this.createFakeQrDownloadLink(index);
            });
        } else {
            // Single click on qr code
            this.createFakeQrDownloadLink(qrIndexes);
        }
    }

    validateState() {
        // Make validations
        this.setState({
            titleInvalid: this.state.title.length <= 0, // Title must not be empty
            qrCountInvalid: this.state.qrCount <= 0, // Qr count must be bigger than 0
            winningMessageInvalid: this.state.winningMessage.length <= 0, // Check if winning message is not empty
            infoInvalid: this.state.info.length <= 0 // Check if info is not empty
        });

        // Check if state is invalid
        if (this.state.title.length <= 0 || this.state.qrCount <= 0 || this.state.winningMessage.length <= 0 || this.state.info.length <= 0) {
            // Push notification
            this.props.pushNotification('danger', 'Prosím zkontrolujte, zda jsou všechna pole vyplněna');
            return false;
        }

        // State is valid return true
        return true;
    }

    handleOnSubmit() {
        // Check if state is valid
        if (!this.validateState()) {
            return;
        }

        // Clone event
        var event = _.cloneDeep(this.props.event);

        // Add game data
        event.game = {
            title: this.state.title,
            info: this.state.info,
            winningMessage: this.state.winningMessage,
            qrCount: this.state.qrCount
        };

        // Update event
        this.props.updateEvent(event);

        // Close modal
        this.props.closeEventsGameModal();
    }

    resetModalState() {
        this.setState({
            title: !_.isNull(this.props.event.game) ? this.props.event.game.title : '',
            info: !_.isNull(this.props.event.game) ? this.props.event.game.info : '',
            winningMessage: !_.isNull(this.props.event.game) ? this.props.event.game.winningMessage : '',
            qrCount: !_.isNull(this.props.event.game) ? this.props.event.game.qrCount : 3
        });
    }

    isNewGame() {
        // Check if it's first time we're creating the game
        return _.isNull(this.props.event.game);
    }

    handleOnDelete() {
        this.setState({
            deleteConfirmVisible: true
        });
    }

    handleOnDeleteConfirm(value) {
        if (value) {
            // Clone event
            var event = _.cloneDeep(this.props.event);

            // Set game to null
            event.game = null;

            // Update event
            this.props.updateEvent(event);

            // Close modal
            this.props.closeEventsGameModal();
        }

        // Hide confirm modal window
        this.setState({
            deleteConfirmVisible: false
        });
    }

    getQrDataFormat(qrId) {
        return JSON.stringify({
            type: 'game',
            data: {
                eventId: this.props.event.id,
                qrId: qrId
            }
        });
    }

    render() {
        // Don't render if it's not visible
        if (!this.props.visible) {
            return null;
        }

        return (
            <Modal
                show={this.props.visible}
                onEnter={() => this.resetModalState()}
                bsSize="lg"
                className="modal-primary events-game-modal"
            >
                <ModalHeader>
                    <ModalTitle>{this.isNewGame() ? 'Vytvořit' : 'Editovat'} soutěž</ModalTitle>
                </ModalHeader>
                <ModalBody>
                    <Input
                        label="Název hry:"
                        type="text"
                        bsStyle={this.state.titleInvalid ? 'error' : null}
                        value={this.state.title}
                        onChange={(event) => this.linkState('title', event.target.value)}
                    />
                    <Input
                        label="Počet QR kódů:"
                        type="text"
                        bsStyle={this.state.qrCountInvalid ? 'error' : null}
                        value={this.state.qrCount}
                        onChange={(event) => this.linkState('qrCount', _.isFinite(parseInt(event.target.value)) ? parseInt(event.target.value) : 0)}
                    />
                    <div className="events-game-modal-qrcodes" id="gameQrCodesWrapper">
                        {_.map(_.range(this.state.qrCount), (qrId) => {
                            return (
                                <span onClick={() => this.handleOnDownloadClick(qrId)} key={qrId}>
                                    <QRCode
                                        value={this.getQrDataFormat(qrId)}
                                        size={131}
                                    />
                                </span>
                            );
                        })}
                    </div>
                    {this.state.qrCount > 0 &&
                        <Button
                            bsStyle="success"
                            onClick={() => this.handleOnDownloadClick(_.range(this.state.qrCount))}
                            className="m-b-10"
                        >
                            <i className="fa fa-download"></i> Stáhnout vše
                        </Button>
                    }
                    <Input
                        type="text"
                        label="Krátká výherní zpráva, která se zobrazí při načtení všech QR kódů:"
                        placeholder="Gratulujeme k výhře, vyzvedněte si cenu na informacích!"
                        bsStyle={this.state.winningMessageInvalid ? 'error' : null}
                        value={this.state.winningMessage}
                        onChange={(event) => this.linkState('winningMessage', event.target.value)}
                    />
                    <div className={classNames('form-group', {'has-error': this.state.infoInvalid})}>
                        <label className="control-label">Informace o hře:</label>
                        <RichEditor
                            className="form-control"
                            editorState={this.state.info}
                            initialEditorState={this.state.info}
                            onChange={(editorState) => this.linkState('info', editorState)}
                        />
                    </div>

                    <Confirm
                        id="events-game-confirm-modal"
                        title="Smazat soutěž"
                        backdrop={false}
                        show={this.state.deleteConfirmVisible}
                        onConfirm={(value) => this.handleOnDeleteConfirm(value)}
                    >
                        Opravdu chcete tuto soutěž smazat?
                    </Confirm>
                </ModalBody>
                <ModalFooter>
                    {!this.isNewGame() &&
                        <Button onClick={() => this.handleOnDelete()} bsStyle="danger" className="pull-left">
                            <span className="fa fa-trash"></span> Smazat soutěž
                        </Button>
                    }
                    <Button onClick={() => this.handleOnSubmit()} bsStyle="primary">{this.isNewGame() ? 'Vytvořit' : 'Editovat'} soutěž</Button>
                    <Button onClick={() => this.props.closeEventsGameModal()}>Zavřít</Button>
                </ModalFooter>
            </Modal>
        );
    }
}

EventsGameModal.propTypes = {
    visible: React.PropTypes.bool,
    event: React.PropTypes.object,

    // Functions
    closeEventsGameModal: React.PropTypes.func,
    pushNotification: React.PropTypes.func,
    updateEvent: React.PropTypes.func
};

export default connect(
    eventsGameModalSelector,
    (dispatch) => {
        return bindActionCreators({ closeEventsGameModal, pushNotification, updateEvent }, dispatch);
    }
)(EventsGameModal)
