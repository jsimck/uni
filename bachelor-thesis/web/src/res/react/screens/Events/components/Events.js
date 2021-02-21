import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Panel, PanelBody, PanelHeader, Confirm, RenderInBody, Notifications, QRModal } from '../../../components'
import { setEvents } from '../actions/events'
import { setMaps } from '../../Maps/actions/maps'
import MapsParser from '../../Maps/helpers/MapsParser'
import EventsTable from './EventsTable'
import EventsTableToolbar from './EventsTableToolbar'
import EventsEditModal from './EventsEditModal'
import EventsGameModal from './EventsGameModal'

class Events extends Component {
    componentDidMount() {
        // Set maps & events
        this.props.setEvents(window.events);
        this.props.setMaps(MapsParser.parse(window.maps));
    }

    render() {
        return (
            <Panel>
                <PanelHeader title="Události"></PanelHeader>
                <PanelBody>
                    <EventsTableToolbar />
                    <EventsTable />
                    <EventsEditModal />
                    <EventsGameModal />
                    <Notifications />
                    <QRModal />
                </PanelBody>
            </Panel>
        );
    }
}

Events.propTypes = {
    setEvents: React.PropTypes.func,
    setMaps: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ setEvents, setMaps }, dispatch);
    }
)(Events)