import React, { Component } from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Button, Input, OverlayTrigger, Tooltip } from 'react-bootstrap'
import { openModal } from '../actions/editModal'
import { openQrModal } from '../../../actions/qrModal'

class EventsTableToolbar extends Component
{
    render() {
        return (
            <div className="text-right m-t-10 m-b-10">
                <Button bsStyle="primary" onClick={() => this.props.openModal(null)}>
                    <i className="pg pg-plus"></i> Vytvořit událost
                </Button>
            </div>
        );
    }
}

EventsTableToolbar.propTypes = {
    openModal: React.PropTypes.func,
    openQrModal: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ openModal, openQrModal }, dispatch);
    }
)(EventsTableToolbar);



