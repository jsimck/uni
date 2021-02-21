import React, { Component } from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { Modal, ModalBody, ModalFooter, ModalHeader, ModalTitle, Button } from 'react-bootstrap'
import QRCode from 'qrcode.react'
import { qrModalSelector } from '../selectors/qrModal'
import { closeQrModal } from '../actions/qrModal'

class QRModal extends Component
{
    handleOnDownloadClick() {
        // Create invisible link to download qr code as img
        var downloadLink = document.createElement('a');
        downloadLink.href = document.querySelector('#qrCodeWrapper').children[0].toDataURL();
        downloadLink.download = 'qrcode';

        // Attach to dom and remove after click
        document.body.appendChild(downloadLink);
        downloadLink.click();
        document.body.removeChild(downloadLink);
    }

    render() {
        return (
            <Modal className="modal-primary" show={this.props.visible}>
                <ModalHeader>
                    <ModalTitle>Generovaný QR kód</ModalTitle>
                </ModalHeader>
                <ModalBody className="text-center" id="qrCodeWrapper">
                    <QRCode size={300} value={this.props.data} />
                </ModalBody>
                <ModalFooter>
                    <Button bsStyle="primary" onClick={() => this.handleOnDownloadClick()}>
                        Stáhnout
                    </Button>
                    <Button onClick={this.props.closeQrModal}>
                        Zavřít
                    </Button>
                </ModalFooter>
            </Modal>
        );
    }
}

QRModal.propTypes = {
    visible: React.PropTypes.bool,
    data: React.PropTypes.string,

    // Functions
    closeQrModal: React.PropTypes.func
};

export default connect(
    qrModalSelector,
    (dispatch) => {
        return bindActionCreators({ closeQrModal }, dispatch);
    }
)(QRModal)
