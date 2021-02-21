import React, { Component } from 'react'
import _ from 'lodash'
import { connect } from 'react-redux'
import moment from 'moment'
import momentLocale from 'moment/locale/cs'
import { Modal, ModalBody, ModalFooter, ModalHeader, ModalTitle, Button } from 'react-bootstrap'
import { eventsMapPickerSelector } from '../selectors/eventsMapPicker'

class EventsMapPicker extends Component {
    constructor(props) {
        super(props);

        this.state = {
            selectedId: null
        };
    }

    resetModalState() {
        this.setState({
            selectedId: this.props.selected
        });
    }

    handleOnRowClick(map) {
        // UnCheck if id is the same as selected
        this.setState({
            selectedId: (map.id == this.state.selectedId) ? null : map.id
        });
    }

    render() {
        return (
            <Modal
                onEnter={() => this.resetModalState()}
                backdrop={false}
                className="modal-primary modal-stacked"
                show={this.props.show}
            >
                <ModalHeader>
                    <ModalTitle>Vybrat mapu</ModalTitle>
                </ModalHeader>

                <ModalBody>
                    <table className="table-condensed table-hover table m-b-0">
                        <thead>
                        <tr>
                            <th style={{width: 58}} className="p-t-0 text-master-dark text-center">
                                #
                            </th>
                            <th className="p-t-0 text-master-dark">
                                Název mapy
                            </th>
                            <th style={{width: 230}} className="p-t-0 text-master-dark text-center">
                                Vytvořeno
                            </th>
                        </tr>
                        </thead>
                        <tbody>
                        {_.map(this.props.maps, currentMap => {
                            return (
                                <tr onClick={() => this.handleOnRowClick(currentMap)} key={currentMap.id} className="cursor-pointer">
                                    <td className="p-t-5 p-b-5 text-center">
                                        <div className="checkbox-fix checkbox check-primary">
                                            <input type="checkbox" checked={this.state.selectedId == currentMap.id} value="1" />
                                            <label></label>
                                        </div>
                                    </td>
                                    <td className="p-t-5 p-b-5">
                                        {currentMap.title}
                                    </td>
                                    <td className="p-t-5 p-b-5 text-center">
                                        {moment(currentMap.created).format('LL')}
                                    </td>
                                </tr>
                            );
                        })}
                        </tbody>
                    </table>
                </ModalBody>
                <ModalFooter>
                    <Button onClick={() => this.props.onAssign(this.state.selectedId)} bsStyle="primary">Potvrdit</Button>
                    <Button onClick={() => this.props.onAssign(this.state.selectedId)}>Zavřít</Button>
                </ModalFooter>
            </Modal>
        );
    }
}

EventsMapPicker.propTypes = {
    maps: React.PropTypes.array,
    onAssign: React.PropTypes.func
};

export default connect(
    eventsMapPickerSelector
)(EventsMapPicker)