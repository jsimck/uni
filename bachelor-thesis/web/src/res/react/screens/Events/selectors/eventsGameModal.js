import _ from 'lodash'
import { createSelector } from 'reselect'

export const eventsGameModalSelector = createSelector(
    state => state.eventsGameModal.visible,
    state => state.eventsGameModal.eventId,
    state => state.events.events,
    (visible, eventId, events) => {
        return {
            visible: visible,
            event: _.find(events, {id: eventId})
        };
    }
);