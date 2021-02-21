import React, { Component } from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { Panel, PanelBody, PanelHeader, RenderInBody, QRModal, Notifications } from '../../../components'
import { setMap } from '../actions/canvas'
import { setEvents } from '../../Events/actions/events'
import MapsParser from '../../Maps/helpers/MapsParser'
import { mapEditorSelector } from '../selectors/mapEditor'
import Canvas from './Canvas'
import CanvasSidebar from './CanvasSidebar'
import CanvasToolbar from './CanvasToolbar'
import OptionsSidebar from './OptionsSidebar'
import EditActionModal from './EditActionModal'

class MapEditor extends Component {
    componentDidMount() {
        // Set map & events
        this.props.setEvents(window.events);
        this.props.setMap(MapsParser.parseSingle(window.map));
    }

    render() {
        return (
            <Panel>
                <PanelHeader title={'Editor mapy - ' + this.props.map.title}></PanelHeader>
                <PanelBody className="p-0 map-editor">
                    <CanvasToolbar />
                    <CanvasSidebar />
                    <Canvas />
                    <OptionsSidebar />
                    <EditActionModal />
                    <Notifications />
                    <QRModal />
                </PanelBody>
            </Panel>
        );
    }
}

MapEditor.propTypes = {
    map: React.PropTypes.object,

    // Functions
    setMap: React.PropTypes.func,
    setEvents: React.PropTypes.func
};

export default connect(
    mapEditorSelector,
    (dispatch) => {
        return bindActionCreators({ setMap, setEvents }, dispatch);
    }
)(MapEditor)