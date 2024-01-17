import pandas as pd

# Specify the full path to your CSV file
csv_file_path = 'C:/Users/pc/ddcd/webpage_algorithm/dataset.csv'

# Read the CSV file with the correct encoding
df = pd.read_csv(csv_file_path, encoding='euc-kr')

# Specify columns to merge
columns_to_merge = ['17', '18', '19', '20', '21', '22', '23']

# Create a new column with merged values
df['Merged_Column'] = df[columns_to_merge].apply(lambda x: ' '.join(x.dropna().astype(str)), axis=1)

# Drop the original columns
df = df.drop(columns=columns_to_merge)

# Specify the full path to save the merged file
output_file_path = 'C:/Users/pc/ddcd/webpage_algorithm/merged_file.csv'

# Save the DataFrame to a new CSV file
df.to_csv(output_file_path, index=False)