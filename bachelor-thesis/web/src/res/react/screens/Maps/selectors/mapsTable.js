import { createStructuredSelector } from 'reselect'

export const mapsTableSelector = createStructuredSelector({
    maps: state => state.maps.maps,
    processing: state => state.maps.processing,
    removeId: state => state.removeMapConfirm.removeId,
    showRemoveConfirm: state => state.removeMapConfirm.showRemoveConfirm
});