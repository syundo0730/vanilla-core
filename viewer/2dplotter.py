import plotly.offline as offline
import plotly.graph_objs as go
import pandas as pd
import argparse


def main():
    offline.init_notebook_mode(connected=False)

    parser = argparse.ArgumentParser()
    parser.add_argument('-f', required=True)
    args = parser.parse_args()
    file_name = args.f
    raw_names = [
        ['t', 'a0'],
        ['t', 'a1'],
        ['t', 'a2'],
        ['t', 'a3'],
        ['t', 'a4'],
        ['t', 'a5']
    ]
    raw = pd.read_csv(file_name)
    data = list(generate_traces(raw, raw_names))
    offline.plot(data)


def new_trace(_x, _y):
    trace = go.Scatter(
        x=_x,
        y=_y,
        mode='lines+markers'
    )
    return trace


def generate_traces(raw, raw_names):
    for name in raw_names:
        n_0 = name[0]
        n_1 = name[1]
        print(raw[n_0])
        print(raw[n_1])
        yield new_trace(raw[n_0], raw[n_1])

if __name__ == '__main__':
    main()
