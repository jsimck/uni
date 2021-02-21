import { createStructuredSelector } from 'reselect'

export const mapsEditModalSelector = createStructuredSelector({
    processing: state => state.maps.processing,
    isVisible: state => state.mapsEditModal.isVisible,
    map: state => state.mapsEditModal.map
});