import React, { Component } from 'react'
import _ from 'lodash'
import { connect } from 'react-redux'
import { bindActionCreators } from 'redux'
import moment from 'moment'
import classNames from 'classnames'
import { Modal, ModalBody, ModalFooter, ModalHeader, ModalTitle, Button, Input, Row, Col, OverlayTrigger, Tooltip } from 'react-bootstrap'
import DatePicker from 'react-datepicker'
import EventsMapPicker from '../components/EventsMapPicker'
import { ButtonLoading, RichEditor } from '../../../components'
import { pushNotification } from '../../../actions/notifications'
import { eventsModalSelector } from '../selectors/eventsModal'
import { closeModal } from '../actions/editModal'
import { addEvent, updateEvent } from '../actions/events'

class EventsEditModal extends Component {
    constructor(props) {
        super(props);

        this.state = {
            id: null,
            title: '',
            description: '',
            content: null,
            startDate: null,
            endDate: null,
            mapId: null,
            assignVisible: false,
            error: false,
            titleInvalid: false,
            descriptionInvalid: false,
            startDateInvalid: false,
            endDateInvalid: false
        };
    }

    resetModalState() {
        this.setState({
            id: !_.isNull(this.props.event) ? this.props.event.id : null,
            title: !_.isNull(this.props.event) ? this.props.event.title : '',
            description: !_.isNull(this.props.event) ? this.props.event.description : '',
            content: !_.isNull(this.props.event) ? this.props.event.content : null,
            startDate: !_.isNull(this.props.event) ? moment(this.props.event.startDate) : moment(),
            endDate: !_.isNull(this.props.event) ? moment(this.props.event.endDate) : moment(),
            mapId: !_.isNull(this.props.event) ? this.props.event.mapId : null
        });
    }

    validateState() {
        // Make validations
        this.setState({
            titleInvalid: this.state.title.length <= 0,
            descriptionInvalid: this.state.description.length <= 0,
            startDateInvalid: _.isDate(this.state.startDate),
            endDateInvalid: _.isDate(this.state.endDate)
        });

        // Check if state is invalid
        if (this.state.title.length <= 0 || this.state.description.length <= 0 || _.isDate(this.state.startDate) || _.isDate(this.state.endDate)) {
            // Push notification
            this.props.pushNotification('danger', 'Prosím zkontrolujte, zda jsou všechna pole vyplněna');
            return false;
        }

        // State is valid return true
        return true;
    }

    handleAddClick() {
        if (!this.validateState()) {
            return;
        }

        // Update event
        if (!_.isNull(this.state.id)) {
            this.props.updateEvent({
                id: this.state.id,
                title: this.state.title,
                description: this.state.description,
                content: this.state.content,
                startDate: this.state.startDate._d,
                endDate: this.state.endDate._d,
                mapId: this.state.mapId
            });
        } else {
            this.props.addEvent(
                this.state.title,
                this.state.description,
                this.state.content,
                this.state.startDate._d,
                this.state.endDate._d,
                this.state.mapId
            );
        }
    }

    linkState(value, key) {
        this.setState({
            [key]: value
        });
    }

    handleOnAssignShow() {
        this.setState({
            assignVisible: true
        });
    }

    handleOnAssign(mapId) {
        // Hide Modal
        this.setState({
            assignVisible: false,
            mapId: mapId
        });
    }

    render() {
        const tooltipMapPicker = <Tooltip id="events-action-map-picker">Přiřadit mapu</Tooltip>;

        return (
            <div>
                <Modal
                    onEnter={() => this.resetModalState()}
                    bsSize="lg"
                    className="modal-default event-modal"
                    show={this.props.visible}
                >
                    <ModalBody>
                        <div className="meta-wrapper">
                            <Row>
                                <Col xs={10}>
                                    <Input
                                        type="text"
                                        className="title"
                                        placeholder="Název události:"
                                        bsStyle={this.state.titleInvalid ? 'error' : null}
                                        value={this.state.title}
                                        onChange={(e) => this.linkState(e.target.value, 'title')}
                                        label={null}
                                    />
                                </Col>
                                <Col xs={2}>
                                    <OverlayTrigger placement="top" overlay={tooltipMapPicker}>
                                        <Button
                                            onClick={() => this.handleOnAssignShow()}
                                            bsStyle="link"
                                            className="pull-right"
                                        >
                                            <span className="fa-stack fa-lg">
                                                <i className={classNames('fa', 'fa-map', 'fa-stack-2x', {'text-primary': this.state.mapId})}></i>
                                                <i style={{bottom: -13, left: 18}}
                                                   className={classNames('fa', 'fa-stack-1x', 'text-success',
                                                   {'fa-plus': !this.state.mapId}, {'fa-check': this.state.mapId})}
                                                ></i>
                                            </span>
                                        </Button>
                                    </OverlayTrigger>
                                </Col>
                            </Row>
                            <Row>
                                <Col xs={6}>
                                    <div className={classNames('form-group', {'has-error': this.state.startDateInvalid})}>
                                        <label className="control-label">Začátek:</label>
                                        <DatePicker
                                            className="form-control"
                                            selected={this.state.startDate}
                                            startDate={this.state.startDate}
                                            endDate={this.state.endDate}
                                            onChange={(date) => this.linkState(date, 'startDate')}
                                        />
                                    </div>
                                </Col>
                                <Col xs={6}>
                                    <div className={classNames('form-group', {'has-error': this.state.endDateInvalid})}>
                                        <label className="control-label">Konec:</label>
                                        <DatePicker
                                            className="form-control"
                                            selected={this.state.endDate}
                                            startDate={this.state.startDate}
                                            endDate={this.state.endDate}
                                            onChange={(date) => this.linkState(date, 'endDate')}
                                        />
                                    </div>
                                </Col>
                            </Row>
                            <Row className="m-t-10">
                                <Col xs={12}>
                                    <Input
                                        type="textarea"
                                        label="Krátký popis:"
                                        rows="3"
                                        bsStyle={this.state.descriptionInvalid ? 'error' : null}
                                        value={this.state.description}
                                        onChange={(e) => this.linkState(e.target.value, 'description')}
                                    />
                                </Col>
                            </Row>
                        </div>
                        <div className="editor-wrapper">
                            <RichEditor
                                editorState={this.state.content}
                                initialEditorState={this.state.content}
                                onChange={(editorState) => this.linkState(editorState, 'content')}
                            />
                        </div>
                    </ModalBody>

                    <ModalFooter>
                        <ButtonLoading
                            onClick={() => this.handleAddClick()}
                            bsStyle="success"
                            disabled={this.props.processing}
                            loading={this.props.processing}
                        >
                            {this.state.id ? 'Editovat' : 'Vytvořit'} událost
                        </ButtonLoading>
                        <Button onClick={() => this.props.closeModal()}>Zavřít</Button>
                    </ModalFooter>
                </Modal>

                <EventsMapPicker
                    onAssign={(mapId) => this.handleOnAssign(mapId)}
                    selected={this.state.mapId}
                    show={this.state.assignVisible}
                />
            </div>
        );
    }
}

EventsEditModal.propTypes = {
    processing: React.PropTypes.bool,
    visible: React.PropTypes.bool,
    event: React.PropTypes.object,

    // Functions
    closeModal: React.PropTypes.func,
    addEvent: React.PropTypes.func,
    updateEvent: React.PropTypes.func,
    pushNotification: React.PropTypes.func
};

export default connect(
    eventsModalSelector,
    (dispatch) => {
        return bindActionCreators({ closeModal, addEvent, updateEvent, pushNotification }, dispatch);
    }
)(EventsEditModal)