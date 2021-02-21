import API from '../services/APIService'
import { activateNetworkActivity, deactivateNetworkActivity } from '../actions/statusBarActions'
import { pushAlert } from '../actions/notificationAlertActions'

// Manage events actions
export const FETCH_EVENTS_REQUEST = 'FETCH_EVENTS_REQUEST';
export const FETCH_EVENTS_ERROR = 'FETCH_EVENTS_ERROR';
export const FETCH_EVENTS_SUCCESS = 'FETCH_EVENTS_SUCCESS';

export function fetchEventsRequest(refreshing) {
    return {
        type: FETCH_EVENTS_REQUEST,
        refreshing: refreshing
    };
}

export function fetchEventsError() {
    return {
        type: FETCH_EVENTS_ERROR
    };
}

export function fetchEventsSuccess(events) {
    return {
        type: FETCH_EVENTS_SUCCESS,
        events: events
    };
}

export function fetchEvents(refreshing = false) {
    return dispatch => {
        dispatch(fetchEventsRequest(refreshing));
        // Activate statusBar network indicator
        dispatch(activateNetworkActivity());

        // Call API to fetch events
        API.listEvents((success, response) => {
            if (success) {
                dispatch(fetchEventsSuccess(response));
            } else {
                dispatch(fetchEventsError());
                dispatch(pushAlert('Chyba aktualizace událostí', 'Nemohli jsme načíst události ze serveru'));
            }

            // Deactivate statusBar network indicator
            dispatch(deactivateNetworkActivity());
        });
    };
}

// Fetch map actions
export const FETCH_MAP_REQUEST = 'FETCH_MAP_REQUEST';
export const FETCH_MAP_ERROR = 'FETCH_MAP_ERROR';
export const FETCH_MAP_SUCCESS = 'FETCH_MAP_SUCCESS';

export function fetchMapRequest() {
    return {
        type: FETCH_MAP_REQUEST
    };
}

export function fetchMapError() {
    return {
        type: FETCH_MAP_ERROR
    };
}

export function fetchMapSuccess(id, map) {
    return {
        type: FETCH_MAP_SUCCESS,
        eventId: id,
        map: map
    };
}

export function fetchMap(mapId, eventId) {
    return dispatch => {
        dispatch(fetchMapRequest());
        // Activate statusBar network indicator
        dispatch(activateNetworkActivity());

        // Call API to fetch events
        API.getMap(mapId, (success, response) => {
            if (success) {
                dispatch(fetchMapSuccess(eventId, response));
            } else {
                dispatch(fetchMapError());
                dispatch(pushAlert('Chyba aktualizace Mapy', 'Nemohli jsme načíst mapy ze serveru'));
            }

            // Deactivate statusBar network indicator
            dispatch(deactivateNetworkActivity());
        });
    };
}