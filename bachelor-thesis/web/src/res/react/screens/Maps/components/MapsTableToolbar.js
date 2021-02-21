import React  from 'react'
import { Button } from 'react-bootstrap'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { showMapsEditModal } from '../actions/mapsEditModal'

const MapsTableToolbar = (props) => {
    return (
        <div className="text-right m-t-10 m-b-10">
            <Button bsStyle="primary" onClick={() => props.showMapsEditModal()}>
                <i className="pg pg-plus"></i> Vytvořit mapu
            </Button>
        </div>
    );
};

MapsTableToolbar.propTypes = {
    showMapsEditModal: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ showMapsEditModal }, dispatch);
    }
)(MapsTableToolbar)