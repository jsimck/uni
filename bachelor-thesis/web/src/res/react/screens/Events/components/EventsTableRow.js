import React, { Component } from 'react'
import _ from 'lodash'
import moment from 'moment'
import momentLocale from 'moment/locale/cs'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Button, ButtonGroup, OverlayTrigger, Tooltip } from 'react-bootstrap'
import { openQrModal } from '../../../actions/qrModal'
import { removeEventConfirmShow } from '../actions/removeEventConfirm'
import { openEventsGameModal } from '../actions/eventsGameModal'
import { openModal } from '../actions/editModal'

class EventsTableRow extends Component {
    handleOpenQrModal() {
        this.props.openQrModal(JSON.stringify({
            type: 'event',
            data: {
                id: this.props.event.id
            }
        }));
    }

    render() {
        const tooltipEdit = <Tooltip id={`events-row-${this.props.event.id}-action-edit`}>Editovat událost</Tooltip>;
        const tooltipRemove = <Tooltip id={`events-row-${this.props.event.id}-action-remove`}>Smazat událost</Tooltip>;
        const tooltipQRCode = <Tooltip id={`events-row-${this.props.event.id}-action-qrcode`}>Generovat QR kód</Tooltip>;
        const tooltipGame = <Tooltip id={`events-row-${this.props.event.id}-action-game`}>{_.isNull(this.props.event.game) ? 'Vytvořit' : 'Editovat'} soutěž</Tooltip>;

        return (
            <tr>
                <td className="v-align-middle">
                    <strong>{this.props.event.title}</strong>
                </td>
                <td className="v-align-middle text-center">
                    {moment(this.props.event.startDate).format('LL')}
                </td>
                <td className="v-align-middle text-center">
                    {moment(this.props.event.endDate).format('LL')}
                </td>
                <td className="v-align-middle text-center">
                    <ButtonGroup>
                        <OverlayTrigger placement="top" overlay={tooltipGame}>
                            <Button onClick={() => this.props.openEventsGameModal(this.props.event.id)}>
                                <span className="fa fa-gamepad"></span>
                            </Button>
                        </OverlayTrigger>
                        <OverlayTrigger placement="top" overlay={tooltipQRCode}>
                            <Button onClick={() => this.handleOpenQrModal()}>
                                <span className="fa fa-qrcode"></span>
                            </Button>
                        </OverlayTrigger>
                        <OverlayTrigger placement="top" overlay={tooltipEdit}>
                            <Button onClick={() => this.props.openModal(this.props.event)}>
                                <span className="fa fa-pencil"></span>
                            </Button>
                        </OverlayTrigger>
                        <OverlayTrigger placement="top" overlay={tooltipRemove}>
                            <Button onClick={() => this.props.removeEventConfirmShow(this.props.event.id)}>
                                <span className="fa fa-trash"></span>
                            </Button>
                        </OverlayTrigger>
                    </ButtonGroup>
                </td>
            </tr>
        );
    }
}

EventsTableRow.propTypes = {
    event: React.PropTypes.object,

    // Functions
    removeEventConfirmShow: React.PropTypes.func,
    openEventsGameModal: React.PropTypes.func,
    openQrModal: React.PropTypes.func,
    openModal: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ removeEventConfirmShow, openModal, openQrModal, openEventsGameModal }, dispatch);
    }
)(EventsTableRow)
