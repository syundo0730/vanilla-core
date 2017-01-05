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
        ['llx', 'lly', 'llz'],
        ['rlx', 'rly', 'rlz'],
        ['gx', 'gy', 'gz']
    ]
    raw = pd.read_csv(file_name)
    data = list(generate_traces(raw, raw_names))
    layout = go.Layout(
        margin=dict(
            l=0,
            r=0,
            b=0,
            t=0
        )
    )
    fig = go.Figure(data=data, layout=layout)
    offline.plot(fig)


def new_trace(x, y, z):
    trace = go.Scatter3d(
        x=x,
        y=y,
        z=z,
        # mode='markers',
        mode='line',
        marker=dict(
            size=1,
            line=dict(
                color='rgba(0, 0, 0, 0.5)',
                width=0.5
            ),
            opacity=0.8
        )
    )
    return trace


def generate_traces(raw, raw_names):
    for name in raw_names:
        n_x = name[0]
        n_y = name[1]
        n_z = name[2]
        yield new_trace(raw[n_x], raw[n_y], raw[n_z])

if __name__ == '__main__':
    main()
