CREATE TABLE IF NOT EXISTS neurons(
  name TEXT PRIMARY KEY,
  actor BLOB NOT NULL,
  payload BLOB NOT NULL,
  moment INTEGER NOT NULL,
  meta BLOB
);

CREATE INDEX IF NOT EXISTS idx_neurons_moment ON neurons(moment);

CREATE TABLE IF NOT EXISTS synapses(
  name TEXT PRIMARY KEY,
  actor BLOB NOT NULL,
  from_id TEXT NOT NULL,
  to_id TEXT NOT NULL,
  moment INTEGER NOT NULL,
  meta BLOB
);

CREATE INDEX IF NOT EXISTS idx_synapses_moment ON synapses(moment);
CREATE INDEX IF NOT EXISTS idx_synapses_from_to ON synapses(from_id, to_id);

CREATE TABLE IF NOT EXISTS dreams(
  name TEXT PRIMARY KEY,
  actor BLOB NOT NULL,
  neuron TEXT NOT NULL,
  payload BLOB NOT NULL,
  moment INTEGER NOT NULL,
  meta BLOB
);

CREATE INDEX IF NOT EXISTS idx_dreams_moment ON dreams(moment);
CREATE INDEX IF NOT EXISTS idx_dreams_neuron ON dreams(neuron);
