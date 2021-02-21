import { createStructuredSelector } from 'reselect'

export const eventsTableSelector = createStructuredSelector({
    events: state => state.events.events,
    processing: state => state.events.processing,
    removeId: state => state.removeEventConfirm.removeId,
    removeConfirm: state => state.removeEventConfirm.removeConfirm
});