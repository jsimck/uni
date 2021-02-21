import { createStructuredSelector } from 'reselect'

export const eventsMapPickerSelector = createStructuredSelector({
    maps: state => state.maps.maps
});