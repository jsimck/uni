import React from 'react'
import { Modal, ModalBody, ModalHeader, ModalFooter, Button, ModalTitle } from 'react-bootstrap'
import ButtonLoading from './ButtonLoading'

const Confirm = (props) => {
    return (
        <Modal className="modal-danger" id={props.id} show={props.show} backdrop={props.backdrop}>
            <ModalHeader>
                <ModalTitle>{props.title}</ModalTitle>
            </ModalHeader>
            <ModalBody>
                {props.children}
            </ModalBody>
            <ModalFooter>
                <ButtonLoading onClick={() => props.onConfirm(true)} loading={props.loading} disabled={props.disabled} bsStyle="danger">
                    {props.btnTitle}
                </ButtonLoading>
                <Button onClick={() => props.onConfirm(false)}>Zavřít</Button>
            </ModalFooter>
        </Modal>
    );
};

Confirm.propTypes = {
    id: React.PropTypes.string,
    btnTitle: React.PropTypes.string,
    backdrop: React.PropTypes.any,
    show: React.PropTypes.bool,
    loading: React.PropTypes.bool,
    disabled: React.PropTypes.bool,
    children: React.PropTypes.any.isRequired,
    onConfirm: React.PropTypes.func.isRequired
};

Confirm.defaultProps = {
    btnTitle: 'Smazat',
    backdrop: true,
    show: false,
    loading: false,
    disabled: false
};

export default Confirm;