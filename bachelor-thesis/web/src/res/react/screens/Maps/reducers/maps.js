import _ from 'lodash'
import * as actions from '../actions/maps'

const initialState = {
    maps: [],
    processing: false
};

const maps = (state = initialState, action) => {
    switch (action.type) {
        case actions.ADD_MAP_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.ADD_MAP_SUCCESS:
            return {
                ...state,
                processing: false,
                maps: [
                    ...state.maps,
                    action.map
                ]
            };

        case actions.ADD_MAP_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.REMOVE_MAP_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.REMOVE_MAP_SUCCESS:
            var index = _.findIndex(state.maps, {id: action.id});

            return {
                ...state,
                processing: false,
                maps: [
                    ..._.slice(state.maps, 0, index),
                    ..._.slice(state.maps, index + 1)
                ]
            };

        case actions.REMOVE_MAP_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.EDIT_MAP_REQUEST:
            return {
                ...state,
                processing: true
            };

        case actions.EDIT_MAP_SUCCESS:
            var index = _.findIndex(state.maps, {id: action.map.id});

            return {
                ...state,
                processing: false,
                maps: [
                    ..._.slice(state.maps, 0, index),
                    action.map,
                    ..._.slice(state.maps, index + 1)
                ]
            };

        case actions.EDIT_MAP_ERROR:
            return {
                ...state,
                processing: false
            };

        case actions.SET_MAPS:
            return {
                ...state,
                maps: action.maps
            };

        default:
            return state;
    }
};

export default maps