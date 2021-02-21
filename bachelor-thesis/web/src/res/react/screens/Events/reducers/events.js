import _ from 'lodash'
import * as actions from '../actions/events'

const initialState = {
    processing: false,
    events: []
};

const events = (state = initialState, action) => {
    switch (action.type) {
        case actions.ADD_EVENT_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.ADD_EVENT_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.ADD_EVENT_SUCCESS:
            return {
                ...state,
                events: [
                    ...state.events,
                    action.event
                ],
                processing: false
            };

        case actions.REMOVE_EVENT_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.REMOVE_EVENT_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.REMOVE_EVENT_SUCCESS:
            var deleteIndex = _.findIndex(state.events, {id: action.id});

            return {
                ...state,
                events: [
                    ..._.slice(state.events, 0, deleteIndex),
                    ..._.slice(state.events, deleteIndex + 1)
                ],
                processing: false
            };

        case actions.UPDATE_EVENT_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.UPDATE_EVENT_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.UPDATE_EVENT_SUCCESS:
            var deleteIndex = _.findIndex(state.events, {id: action.event.id});

            return {
                ...state,
                events: [
                    ..._.slice(state.events, 0, deleteIndex),
                    action.event,
                    ..._.slice(state.events, deleteIndex + 1)
                ],
                processing: false
            };

        case actions.SET_EVENTS:
            return {
                ...state,
                events: [
                    ...action.events
                ]
            };

        default:
            return state;
    }
};

export default events