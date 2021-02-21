import _ from 'lodash'
import * as actions from '../actions/eventsActions'
import { REHYDRATE } from 'redux-persist/constants'

// 48h cache for events
export const CACHE_EVENTS_PERIOD_H = 48;

const initialState = {
    events: [],
    cachedDate: new Date(),
    cached: true,
    error: false,
    loading: false,
    refreshing: false,
    mapLoading: false
};

const events = (state = initialState, action) => {
    switch (action.type) {
        case actions.FETCH_EVENTS_SUCCESS:
            return {
                ...state,
                loading: false,
                refreshing: false,
                events: [
                    ...action.events
                ]
            };

        case actions.FETCH_EVENTS_ERROR:
            return {
                ...state,
                loading: false,
                refreshing: false,
                cached: false,
                error: true
            };

        case actions.FETCH_EVENTS_REQUEST:
            return {
                ...state,
                loading: true,
                refreshing: action.refreshing,
                cachedDate: new Date(),
                cached: true
            };

        case actions.FETCH_MAP_SUCCESS:
            var index = _.findIndex(state.events, {id: action.eventId});

            return {
                ...state,
                mapLoading: false,
                events: [
                    ...state.events.slice(0, index),
                    {
                        ...state.events[index],
                        map: {
                            ...action.map
                        }
                    },
                    ...state.events.slice(index + 1)
                ]
            };

        case actions.FETCH_MAP_ERROR:
            return {
                ...state,
                mapLoading: false
            };

        case actions.FETCH_MAP_REQUEST:
            return {
                ...state,
                mapLoading: true
            };

        case REHYDRATE:
            // If cache hasn't been set yet, return with cache set to false so we fetch new events
            if (_.isUndefined(action.payload.events) || action.payload.events.events.length <= 0) {
                return {
                    ...state,
                    cached: false
                };
            }

            // Parse dates
            var oldCachedDate = new Date(action.payload.events.cachedDate);
            // Add max cached time to oldCacheDate
            oldCachedDate.setHours(oldCachedDate.getHours() + CACHE_EVENTS_PERIOD_H);
            var today = new Date();

            // If data are older than 24hours, we'll fetch them again
            if (today > oldCachedDate) {
                return {
                    ...state,
                    cached: false
                };
            } else {
                // If data are not older than 24 hours, we'll use cache
                return {
                    ...state,
                    events: [
                        ...action.payload.events.events
                    ],
                    cachedDate: new Date(action.payload.events.cachedDate),
                    cached: true
                };
            }

        default:
            return state;
    }
};

export default events