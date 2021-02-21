import React, { Component } from 'react'
import moment from 'moment'
import moment_locale from 'moment/locale/cs'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { Button, ButtonGroup, Tooltip, OverlayTrigger } from 'react-bootstrap'
import { Confirm } from '../../../components'
import { removeMapShowConfirm } from '../actions/removeMapConfirm'
import { showMapsEditModal } from '../actions/mapsEditModal'

class MapsTableRow extends Component {
    redirectToMapEditor(id) {
        document.location = `/maps/${id}/editor`;
    }

    render() {
        // Create tooltips for action buttons
        const tooltipCreate = <Tooltip id={'maps-row-action-create' + this.props.map.id}>Nakreslit mapu</Tooltip>;
        const tooltipEdit = <Tooltip id={'maps-row-action-edit' + this.props.map.id}>Editovat mapu</Tooltip>;
        const tooltipDelete = <Tooltip id={'maps-row-action-delete' + this.props.map.id}>Smazat mapu</Tooltip>;

        return (
            <tr>
                <td>
                    <strong>{this.props.map.title}</strong>
                </td>
                <td>
                    {this.props.map.description}
                </td>
                <td className="text-center">
                    {this.props.map.updated ? moment(this.props.map.updated).format('LL') : null}
                </td>
                <td className="text-center">
                    {moment(this.props.map.created).format('LL')}
                </td>
                <td>
                    <ButtonGroup className="btn-group-sm">
                        <OverlayTrigger placement="top" overlay={tooltipCreate}>
                            <Button onClick={() => this.redirectToMapEditor(this.props.map.id)}>
                                <i className="fa fa-map"></i>
                            </Button>
                        </OverlayTrigger>
                        <OverlayTrigger placement="top" overlay={tooltipEdit}>
                            <Button onClick={() => this.props.showMapsEditModal(this.props.map)}>
                                <i className="fa fa-pencil"></i>
                            </Button>
                        </OverlayTrigger>
                        <OverlayTrigger placement="top" overlay={tooltipDelete}>
                            <Button onClick={() => this.props.removeMapShowConfirm(this.props.map.id)}>
                                <i className="fa fa-trash"></i>
                            </Button>
                        </OverlayTrigger>
                    </ButtonGroup>
                </td>
            </tr>
        );
    }
}

MapsTableRow.propTypes = {
    map: React.PropTypes.object,

    // Functions
    removeMapShowConfirm: React.PropTypes.func,
    showMapsEditModal: React.PropTypes.func
};

export default connect(
    null,
    (dispatch) => {
        return bindActionCreators({ removeMapShowConfirm, showMapsEditModal }, dispatch);
    }
)(MapsTableRow)