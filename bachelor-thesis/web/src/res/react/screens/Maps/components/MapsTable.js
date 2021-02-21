import React, { Component } from 'react';
import _ from 'lodash'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import MapsTableRow from './MapsTableRow'
import { Confirm } from '../../../components'
import { removeMap } from '../actions/maps'
import { removeMapHideConfirm } from '../actions/removeMapConfirm'
import { mapsTableSelector } from '../selectors/mapsTable'

class MapsTable extends Component {
    handleDeleteConfirm(confirm) {
        if (confirm) {
            this.props.removeMap(this.props.removeId);
        }

        this.props.removeMapHideConfirm();
    }

    render() {
        return (
            <div>
                <table className="table m-0 full-width table-condensed">
                    <thead>
                    <tr>
                        <th className="text-master-dark" style={{width: '25%'}}>
                            Název
                        </th>
                        <th className="text-master-dark">
                            Popis
                        </th>
                        <th className="text-master-dark text-center" style={{width: '190px'}}>
                            Aktualizováno
                        </th>
                        <th className="text-master-dark text-center" style={{width: '190px'}}>
                            Vytvořeno
                        </th>
                        <th className="text-master-dark text-center" style={{width: '172px'}}>
                            Akce
                        </th>
                    </tr>
                    </thead>
                    <tbody>
                        {_.map((_.sortBy(this.props.maps, (o) => new Date(o.created))).reverse(), (item) => {
                            return <MapsTableRow key={item.id} map={item} />;
                        })}
                    </tbody>
                </table>

                <Confirm
                    title="Smazat mapu"
                    disabled={this.props.processing}
                    loading={this.props.processing}
                    onConfirm={(confirm) => this.handleDeleteConfirm(confirm)}
                    show={this.props.showRemoveConfirm}
                >
                    Opravdu chcete smazat tuto mapu?
                </Confirm>
            </div>
        );
    }
}

MapsTable.propTypes = {
    maps: React.PropTypes.array,
    removeId: React.PropTypes.number,
    processing: React.PropTypes.bool,
    showRemoveConfirm: React.PropTypes.bool,

    // Functions
    removeMapHideConfirm: React.PropTypes.func,
    removeMap: React.PropTypes.func
};

export default connect(
    mapsTableSelector,
    (dispatch) => {
        return bindActionCreators({ removeMapHideConfirm, removeMap }, dispatch);
    }
)(MapsTable);
