import _ from 'lodash'
import { createSelector } from 'reselect'

export const eventSelector = createSelector(
    state => state.events.events,
    state => state.games.games,
    (state, props) => props.eventId,
    (events, games, eventId) => {
        var event = _.find(events, {id: eventId});
        var game = _.find(games, {id: eventId});

        if (_.isUndefined(game)) {
            // Set default data for game
            game = {
                eventId: eventId,
                qrIds: []
            };
        }

        return {
            map: event.map || {},
            event: event,
            game: game
        };
    }
);