import React, { Component } from 'react'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import { Panel, PanelBody, PanelHeader, RenderInBody, Notifications } from '../../../components'
import MapsEditModal from '../components/MapsEditModal'
import MapsTable from './MapsTable'
import MapsTableToolbar from './MapsTableToolbar'
import MapsParser from '../helpers/MapsParser'
import { setMaps } from '../actions/maps'

class Maps extends Component {
    componentDidMount() {
        // Set maps
        this.props.setMaps(MapsParser.parse(window.maps));
    }

    render() {
        return (
            <Panel>
                <PanelHeader title="Maps" />
                <PanelBody>
                    <MapsTableToolbar />
                    <MapsTable />
                    <Notifications />
                    <MapsEditModal />
                </PanelBody>
            </Panel>
        );
    }
}

Maps.propTypes = {
    setMaps: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ setMaps }, dispatch);
    }
)(Maps)