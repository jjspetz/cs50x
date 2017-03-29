class Analyzer():
    """Implements sentiment analysis."""

    def __init__(self, positives, negatives):
        """Initialize Analyzer."""
        pos_words = []
        neg_words = []

        File_pos = open(positives, "r")
        for lines in File_pos:
            if not lines.startswith(";") and not lines.startswith("\n"):
                pos_words.append(lines.strip("\n"))
        self.positives = pos_words

        File_neg = open(negatives, "r")
        for lines in File_neg:
            if not lines.startswith(";") and not lines.startswith("\n"):
                neg_words.append(lines.strip("\n"))
        self.negatives = neg_words

    def analyze(self, text):
        """Analyze text for sentiment, returning its score."""
        score = 0

        if text in self.positives:
            score +=1
        if text in self.negatives:
            score -= 1

        return score
