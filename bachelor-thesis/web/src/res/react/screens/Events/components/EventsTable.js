import React, { Component } from 'react'
import _ from 'lodash'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Confirm } from '../../../components'
import EventsTableRow from '../components/EventsTableRow'
import { eventsTableSelector } from '../selectors/eventsTable'
import { removeEvent } from '../actions/events'
import { removeEventConfirmClose } from '../actions/removeEventConfirm'

class EventsTable extends Component {
    handleRemoveConfirm(confirm) {
        if (confirm) {
            this.props.removeEvent(this.props.removeId);
        }

        // Hide confirm
        this.props.removeEventConfirmClose();
    }

    render() {
        return (
            <div>
                <table className="table m-0 full-width table-condensed">
                    <thead>
                    <tr>
                        <th className="text-master-dark" style={{width: '35%'}}>
                            Název
                        </th>
                        <th className="text-master-dark text-center" style={{width: '15%'}}>
                            Začátek
                        </th>
                        <th className="text-master-dark text-center" style={{width: '15%'}}>
                            Konec
                        </th>
                        <th className="text-master-dark text-center" style={{width: '191px'}}>
                            Akce
                        </th>
                    </tr>
                    </thead>
                    <tbody>
                    {_.map((_.sortBy(this.props.events, (o) => new Date(o.startDate))).reverse(), (event) => {
                        return <EventsTableRow key={event.id} event={event} />
                    })}
                    </tbody>
                </table>

                <Confirm
                    show={this.props.removeConfirm}
                    onConfirm={(confirm) => this.handleRemoveConfirm(confirm)}
                    title="Smazat událost"
                    disabled={this.props.processing}
                    loading={this.props.processing}
                >
                    Opravdu chcete tuto událost smazat?
                </Confirm>
            </div>
        );
    }
}

EventsTable.propTypes = {
    events: React.PropTypes.array,
    removeId: React.PropTypes.number,
    processing: React.PropTypes.bool,
    removeConfirm: React.PropTypes.bool,

    // Functions
    removeEventConfirmClose: React.PropTypes.func,
    removeEvent: React.PropTypes.func
};

export default connect(
    eventsTableSelector,
    (dispatch) => {
        return bindActionCreators({ removeEventConfirmClose, removeEvent }, dispatch);
    }
)(EventsTable)