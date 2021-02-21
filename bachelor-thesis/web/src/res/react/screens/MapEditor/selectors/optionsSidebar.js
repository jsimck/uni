import _ from 'lodash'
import { createSelector } from 'reselect'

export const optionsSidebarSelector = createSelector(
    state => state.canvas.selectedSvgIndexes,
    state => state.canvas.map.id,
    state => state.canvas.map.mapData.svg,
    state => state.events.events,
    state => state.canvas.map,
    (selectedSvgIndexes, id, svg, events, map) => {
        var filteredEvents = _.filter(events, function (e) {
            return !_.isNull(e.mapId) && e.mapId == id;
        });

        return {
            selectedSvgIndexes: selectedSvgIndexes,
            svg: svg[selectedSvgIndexes[0]],
            events: filteredEvents,
            map: map
        };
    }
);
