export const POINT_LADDERS = {
  individual: [100, 200, 300, 500, 1000, 2000, 4000, 8000, 16000, 32000, 64000, 125000, 250000, 500000, 1000000],
  rapidfire: 500,
};

const baseIndividual = [
  {
    id: 'i1',
    text: 'Which planet is known as the Red Planet?',
    options: ['Venus', 'Mars', 'Jupiter', 'Mercury'],
    correctIndex: 1,
  },
  {
    id: 'i2',
    text: 'What is the largest ocean on Earth?',
    options: ['Atlantic', 'Indian', 'Arctic', 'Pacific'],
    correctIndex: 3,
  },
  {
    id: 'i3',
    text: 'Who painted the Mona Lisa?',
    options: ['Vincent van Gogh', 'Pablo Picasso', 'Leonardo da Vinci', 'Claude Monet'],
    correctIndex: 2,
  },
  {
    id: 'i4',
    text: 'How many bones are in the adult human body?',
    options: ['206', '150', '306', '99'],
    correctIndex: 0,
  },
  {
    id: 'i5',
    text: 'Which language is the most spoken in the world by native speakers?',
    options: ['English', 'Spanish', 'Hindi', 'Mandarin Chinese'],
    correctIndex: 3,
  },
  {
    id: 'i6',
    text: 'What gas do plants absorb from the atmosphere?',
    options: ['Oxygen', 'Carbon dioxide', 'Nitrogen', 'Hydrogen'],
    correctIndex: 1,
  },
  {
    id: 'i7',
    text: 'What is the fastest land animal?',
    options: ['Lion', 'Cheetah', 'Pronghorn', 'Greyhound'],
    correctIndex: 1,
  },
  {
    id: 'i8',
    text: 'In which year did the Berlin Wall fall?',
    options: ['1985', '1989', '1991', '1994'],
    correctIndex: 1,
  },
  {
    id: 'i9',
    text: 'What is the chemical symbol for gold?',
    options: ['Gd', 'Go', 'Au', 'Ag'],
    correctIndex: 2,
  },
  {
    id: 'i10',
    text: 'Who wrote "Romeo and Juliet"?',
    options: ['Charles Dickens', 'William Shakespeare', 'Jane Austen', 'Mark Twain'],
    correctIndex: 1,
  },
  {
    id: 'i11',
    text: 'Which country has the largest population?',
    options: ['China', 'United States', 'India', 'Indonesia'],
    correctIndex: 2,
  },
  {
    id: 'i12',
    text: 'What is the hardest natural substance on Earth?',
    options: ['Titanium', 'Quartz', 'Diamond', 'Granite'],
    correctIndex: 2,
  },
  {
    id: 'i13',
    text: 'Which river is the longest in the world?',
    options: ['Amazon', 'Nile', 'Yangtze', 'Mississippi'],
    correctIndex: 1,
  },
  {
    id: 'i14',
    text: 'How many players are on a standard basketball team on court at once?',
    options: ['5', '6', '7', '11'],
    correctIndex: 0,
  },
  {
    id: 'i15',
    text: 'What year did humans first land on the Moon?',
    options: ['1965', '1967', '1969', '1971'],
    correctIndex: 2,
  },
];

const baseRapid = [
  {
    id: 'r1',
    text: 'What is the capital of Japan?',
    options: ['Seoul', 'Beijing', 'Tokyo', 'Bangkok'],
    correctIndex: 2,
  },
  {
    id: 'r2',
    text: 'How many continents are there on Earth?',
    options: ['5', '6', '7', '8'],
    correctIndex: 2,
  },
  {
    id: 'r3',
    text: 'What is the largest mammal in the world?',
    options: ['Elephant', 'Blue whale', 'Giraffe', 'Orca'],
    correctIndex: 1,
  },
  {
    id: 'r4',
    text: 'Which instrument has 88 keys?',
    options: ['Guitar', 'Violin', 'Piano', 'Flute'],
    correctIndex: 2,
  },
  {
    id: 'r5',
    text: 'What is the freezing point of water in Celsius?',
    options: ['0', '32', '100', '-10'],
    correctIndex: 0,
  },
  {
    id: 'r6',
    text: 'Which country is famous for the Great Pyramid of Giza?',
    options: ['Mexico', 'Egypt', 'Peru', 'Greece'],
    correctIndex: 1,
  },
  {
    id: 'r7',
    text: 'What do bees produce?',
    options: ['Milk', 'Honey', 'Wax', 'Silk'],
    correctIndex: 1,
  },
  {
    id: 'r8',
    text: 'Which planet has the most moons?',
    options: ['Jupiter', 'Saturn', 'Mars', 'Earth'],
    correctIndex: 1,
  },
  {
    id: 'r9',
    text: 'What is the largest desert in the world?',
    options: ['Sahara', 'Gobi', 'Antarctic', 'Arabian'],
    correctIndex: 2,
  },
  {
    id: 'r10',
    text: 'Which sport is played at Wimbledon?',
    options: ['Golf', 'Tennis', 'Cricket', 'Rugby'],
    correctIndex: 1,
  },
];

const shuffle = (arr) => {
  const a = [...arr];
  for (let i = a.length - 1; i > 0; i--) {
    const j = Math.floor(Math.random() * (i + 1));
    [a[i], a[j]] = [a[j], a[i]];
  }
  return a;
};

const buildRound = (round, pool) => {
  return shuffle(pool).map((q, i) => ({
    ...q,
    points: round === 'individual' ? POINT_LADDERS.individual[i] : POINT_LADDERS.rapidfire,
  }));
};

export const QUESTIONS = {
  individual: buildRound('individual', baseIndividual),
  rapidfire: buildRound('rapidfire', baseRapid),
};
