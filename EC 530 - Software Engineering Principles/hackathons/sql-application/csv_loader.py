"""
    This file contains a function to load a CSV file into a pandas DataFrame.
"""

import pandas as pd

def load_csv_to_df(csv_path):
    try:
        df = pd.read_csv(csv_path)
        print(f"CSV loaded successfully with {len(df)} rows and {len(df.columns)} columns.")
        return df
    except Exception as e:
        raise ValueError(f"Failed to load CSV: {e}")
